#pragma once
#define TOWERS_H_INCLUDED
#include <iostream>
#include <vector>
using namespace std;
namespace TowerNS
{

	class Land;
	class BLG;
	class Enemy;
	class Level;
	class Meat;

	template <class T>
	class Vector
	{
		T* data;
		int size;
		int cap;
	public:
		Vector(int = 1);
		Vector(Vector&);
		Vector operator=(const Vector&);
		bool operator==(const Vector&);
		int Capacity() const;
		int Size() const;
		T& operator [](int);
		bool operator==(int);
		void push_back(T);
		void clear();
		void create(const int& i);
		void remove(const int& i);
		void resize(const int& n);
		~Vector<T>(void);

	};

	struct Coord
	{
		int x;
		int y;
		Coord(int x0 = 100, int y0 = 50) :x(x0), y(y0) {}
	};

	struct ChrTw
	{
		int Rad;
		int Dam;
		int RateFire;
		int Cost;
		ChrTw(int Rad1 = 5, int Dam1 = 1, int RateFire1 = 1, int Cost1 = 1) :Rad(Rad1), Dam(Dam1), RateFire(RateFire1), Cost(Cost1) {}
	};


	struct ChrCst
	{
		int Proc;
		int MaxHP;
		int RateRepair;
		int Cost;
		ChrCst(int Proc1 = 1, int MaxHP10 = 10, int RateRepair1 = 1, int Cost1 = 1) :Proc(Proc1), MaxHP(MaxHP10), RateRepair(RateRepair1), Cost(Cost1) {}
	};



	class Land
	{
	private:
		Coord XY;
		int Type;//0 равнина, 1 водоем, 2 гора, 3 стена, 4 башн€, 5 логово, 6 «амок
		BLG* Bptr;
		Enemy* Eptr;
		//Vector<Enemy*> EPTR;
	public:
		Land(int x = 0, int y = 0, int type = 0);
		Land(const Land&);
		Land& operator = (const Land&);
		int GetType() { return Type; };
		Land& SetType(int type) { Type = type; return *this; };
		BLG* GetBLG() { return Bptr; };
		Land& SetBLG(BLG* ptr, int type) { Bptr = ptr; Type = type; return *this; };
		Enemy* GetEnemy() { return Eptr; };
		Land& SetEnemy(Enemy* ptrE) { Eptr = ptrE; return *this; };
		Coord GetCoord() { return XY; };
	};


	class BLG
	{
	public:
		virtual void SetLand(Land* L) = 0;
	protected:
		Land* Lptr;
	};


	class Tower :public BLG
	{
	public:
		virtual void SetLand(Land* L) { Lptr = L; };
		Tower(ChrTw* t = nullptr);
		int LVLUP() { LVL++; return LVL; };
		int DmgEn(Land** Map, Coord Size);
	private:

		int LVL;
		ChrTw* Tabl;
		Vector<ChrTw> Tab;
	};

	class Castle :public BLG
	{
	public:
		virtual void SetLand(Land* L) { Lptr = L; };
		Castle(ChrCst* t = nullptr);
		int LVLUP() { return ++LVL; };
		int GetDmg(Enemy* en);
	private:
		const char Name[13] = "Simple name";
		int LVL;
		int CurHP;
		ChrCst* Tabl;
	};

	class Lair :public BLG
	{
	public:
		virtual void SetLand(Land* L) { Lptr = L; };
		Lair(Enemy** TablRelz1 = nullptr, int* TimeRelz1 = nullptr);
		Enemy* Relz(Land** Map, Coord Size);
		Enemy* RelzTime(Land** Map, Coord Size, int time);
		void Reset() { CurEn = 0; };
	private:
		int CurEn;
		int SZ;
		Enemy** TablRelz;
		int* TimeRelz;
	};



	class Enemy
	{
	public:
		virtual int MakeStep(Land** Map, Coord Size) = 0;
		int GiveDmg() { return CurHP * Dmg; };
		int GetDmg(int Dmg) { CurHP -= Dmg; return CurHP; };
		virtual void Die() = 0;
		virtual void SetLand(Land* L) = 0;
		int GetT() { return Type; };
	protected:
		const char* Name;
		int Type, NAura, MaxHP, CurHP, Regen, Speed, Dmg;
		Land* Lptr;
	};

	class Meat :public Enemy
	{
	public:
		Meat(const char* Name1 = "John", int NAura1 = 0, int MaxHP1 = 10, int Regen1 = 1, int Speed1 = 1, int Dmg1 = 1);
		virtual int MakeStep(Land** Map, Coord Size);
		virtual void SetLand(Land* L) { Lptr = L; };
		virtual void Die() { Lptr = nullptr; }
	};

	class Heavy :public Enemy
	{
	public:
		Heavy(const char* Name1 = "Jack", int NAura1 = 0, int MaxHP1 = 10, int Regen1 = 1, int Speed1 = 1, int Dmg1 = 1);
		virtual int MakeStep(Land** Map, Coord Size);
		virtual void SetLand(Land* L) { Lptr = L; };
		virtual void Die() { Lptr = nullptr;  }
	};

	class Plane :public Enemy
	{
	public:
		Plane(const char* Name1 = "Jamie", int NAura1 = 0, int MaxHP1 = 10, int Regen1 = 1, int Speed1 = 1, int Dmg1 = 1);
		virtual int MakeStep(Land** Map, Coord Size);
		virtual void SetLand(Land* L) { Lptr = L; };
		virtual void Die() { Lptr->SetEnemy(nullptr); Lptr = nullptr; delete this; }
	};

	class Level
	{
	private:
		Coord SZ;
		int ttime;
		Land** Map;
	public:
		Level(Coord Size);
		Coord GetSZ() { return SZ; };
		Level& SetSZ(Coord xy) { SZ = xy; return *this; };
		int GetType(Coord xy) { return Map[xy.x][xy.y].GetType(); };
		Level& SetType(Coord xy, int type) { Map[xy.x][xy.y].SetType(type); return *this; };
		friend std::ostream& operator <<(std::ostream&, Level rd);
		Level& SetBLG(Coord xy, BLG* ptr, int type) { Map[xy.x][xy.y].SetBLG(ptr, type); ptr->SetLand(&Map[xy.x][xy.y]); return *this; };
		Level& SetEnemy(Coord xy, Enemy* ptr) { Map[xy.x][xy.y].SetEnemy(ptr); ptr->SetLand(&Map[xy.x][xy.y]); return *this; };
		void Relize(Lair* cur);
		Enemy* GetEnemy(Coord xy) { return Map[xy.x][xy.y].GetEnemy(); }
		int CheckEn(Enemy* en);
		int Step(Enemy** T, Tower* T1, Tower* T2);
	};






}
