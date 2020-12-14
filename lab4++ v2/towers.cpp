#pragma once
#include"towers.h"
#include <iostream>
#include <windows.h>
#include <vector>
namespace TowerNS
{

	enum ConsoleColor
	{
		Black = 0,
		Blue = 1,
		Green = 2,
		Cyan = 3,
		Red = 4,
		Magenta = 5,
		Brown = 6,
		LightGray = 7,
		DarkGray = 8,
		LightBlue = 9,
		LightGreen = 10,
		LightCyan = 11,
		LightRed = 12,
		LightMagenta = 13,
		Yellow = 14,
		White = 15
	};

	void SetColor(int text)
	{
		HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hStdOut, (WORD)text);
	}

	Land::Land(int x, int y, int type) :XY(x, y), Type(type)
	{
		Bptr = nullptr;
		Eptr = nullptr;
	}

	Land::Land(const Land& st)
	{
		XY = st.XY;
		Type = st.Type;
		Bptr = st.Bptr;
		Eptr = st.Eptr;
	}

	Land& Land::operator=(const Land& st)
	{
		if (this != &st)
		{
			XY = st.XY;
			Type = st.Type;
			Bptr = st.Bptr;
			Eptr = st.Eptr;
		}
		return *this;
	}


	TowerNS::Level::Level(Coord Size)
	{
		Coord D;
		srand(42);
		ttime = 0;
		SZ.x = Size.x;
		SZ.y = Size.y;
		int k;
		Map = new Land * [Size.x];
		for (int i = 0; i < Size.x; i++)
		{
			Map[i] = new Land[Size.y];
			for (int j = 0; j < Size.y; j++)
			{
				k = 1 + rand() % 100;
				if (k > 80)
					k = 2;
				else {
					if (k < 60)
						k = 0;
					else
						k = 1;

				}
				Land h(i, j, k);
				Map[i][j] = h;
			}
		}
	}

	std::ostream& operator << (std::ostream& s, Level rd)
	{
		Coord g;
		g.x = rd.GetSZ().x;
		g.y = rd.GetSZ().y;
		Enemy* h;
		int t, x = g.x, y = g.y;
		for (int j = 0; j < y; j++)
		{
			for (int i = 0; i < x; i++)
			{
				g.x = i; g.y = j;
				h = rd.GetEnemy(g);
				if (h != nullptr) {
					if (h->GetT() == 1)
						t = -1;
					if (h->GetT() == 2)
						t = -2;
					if (h->GetT() == 3)
						t = -3;
					if (h->GetT() == -10)
						t = -10;
				}
				else
					t = rd.GetType(g);
				if (t == -10) {
					SetColor(6);
					s << 'D';
				}
				if (t == -1) {
					SetColor(5);
					s << 'R';
				}
				if (t == -2) {
					SetColor(5);
					s << 'G';
				}
				if (t == -3) {
					SetColor(5);
					s << 'V';
				}
				if (t == 0) {
					SetColor(2);
					s << '=';
				}
				if (t == 1) {
					SetColor(1);
					s << '~';
				}
				if (t == 2) {
					SetColor(8);
					s << '^';
				}
				if (t == 3) {
					SetColor(7);
					s << '#';
				}
				if (t == 4) {
					SetColor(14);
					s << 'H';
				}
				if (t == 5) {
					SetColor(4);
					s << 'O';
				}
				if (t == 6) {
					SetColor(11);
					s << 'M';
				}
				s << ' ';
			}
			s << std::endl;
		}
		SetColor(15);
		return s;
	}

	

	Tower::Tower(ChrTw* t)
	{
		if (t == nullptr)
			throw std::exception();
		Tabl = t;
		LVL = 1;
		Lptr = nullptr;
	}

	int Tower::DmgEn(Land** Map, Coord Size)
	{
		Coord f = Lptr->GetCoord(), g;
		Enemy* en = nullptr;
		for (int i = f.x - Tabl[LVL].Rad; i < f.x + Tabl[LVL].Rad; i++)
		{
			for (int j = f.y - Tabl[LVL].Rad; j < f.y + Tabl[LVL].Rad; j++)
			{
				if (i < Size.x && i >= 0 && j < Size.y && j >= 0)
				{
					if (Map[i][j].GetEnemy() != nullptr) {
						en = Map[i][j].GetEnemy();
						g.x = i; g.y = j;
					}

				}
			}
		}
		if (en == nullptr)
			return -1;
		int EnHP = en->GetDmg(Tabl[LVL].Dam);
		if (EnHP <= 0) {
			Map[g.x][g.y].SetType(-10);
			Map[g.x][g.y].SetEnemy(nullptr);
			en->Die();
			return 0;
		}
		else
			return 1;
	}

	Castle::Castle(ChrCst* t)
	{
		if (t == nullptr)
			throw std::exception();
		Tabl = t;
		LVL = 1;
		Lptr = nullptr;

	}

	int Castle::GetDmg(Enemy* en)
	{
		int DMG = en->GiveDmg();
		CurHP -= DMG;
		if (CurHP <= 0)
			return 0;
		else
			return 1;
	}

	Lair::Lair(Enemy** TablRelz1, int* TimeRelz1)
	{
		if (TablRelz1 == nullptr || TimeRelz1 == nullptr)
			throw std::exception();
		TablRelz = TablRelz1;
		TimeRelz = TimeRelz1;
		CurEn = 0;
		Lptr = nullptr;
		SZ = 3;
	}

	Enemy* Lair::Relz(Land** Map, Coord Size)
	{
		Enemy* Rel = TablRelz[CurEn];
		Lptr->SetEnemy(Rel);
		Rel->SetLand(Lptr);
		CurEn++;
		return Rel;
	}

	Enemy* Lair::RelzTime(Land** Map, Coord Size, int time)
	{
		if (time < TimeRelz[CurEn] || CurEn >= SZ)
			return nullptr;
		Enemy* Rel = TablRelz[CurEn];
		Lptr->SetEnemy(Rel);
		Rel->SetLand(Lptr);
		CurEn++;
		return Rel;
	}

	Meat::Meat(const char* Name1, int NAura1, int MaxHP1, int Regen1, int Speed1, int Dmg1)
	{
		Name = Name1; NAura = NAura1; NAura = NAura1; MaxHP = MaxHP1; Regen = Regen1; Speed = Speed1; Dmg = Dmg1;
		Type = 1; CurHP = MaxHP;
	}


	Heavy::Heavy(const char* Name1, int NAura1, int MaxHP1, int Regen1, int Speed1, int Dmg1)
	{
		Name = Name1; NAura = NAura1; NAura = NAura1; MaxHP = MaxHP1; Regen = Regen1; Speed = Speed1; Dmg = Dmg1;
		Type = 2; CurHP = MaxHP;
	}

	Plane::Plane(const char* Name1, int NAura1, int MaxHP1, int Regen1, int Speed1, int Dmg1)
	{
		Name = Name1; NAura = NAura1; NAura = NAura1; MaxHP = MaxHP1; Regen = Regen1; Speed = Speed1; Dmg = Dmg1;
		Type = 3; CurHP = MaxHP;
	}



	int Meat::MakeStep(Land** Map, Coord Size)
	{
		int** Way, x, y;
		Way = new int* [Size.x];
		for (int i = 0; i < Size.x; i++)
		{
			Way[i] = new int[Size.y];
			for (int j = 0; j < Size.y; j++)
			{
				int T = Map[i][j].GetType();
				if (T == 0 || T == 1 || T == 2)
					Way[i][j] = -1;
				else
					Way[i][j] = -2;
				if (T == 6) {
					Way[i][j] = -6;
					x = i; y = j;
				}
			}
		}
		int d = 0, fl = 0;
		Coord f = Lptr->GetCoord();
		Way[f.x][f.y] = 0;
		do {
			for (int i = 0; i < Size.x; i++)
			{
				for (int j = 0; j < Size.y; j++)
				{
					if (Way[i][j] == d)
					{
						if (i == x && j == y)
							fl = 1;
						else
						{
							if (i - 1 >= 0)
							{
								if (Way[i - 1][j] == -1 || Way[i - 1][j] == -6)
									Way[i - 1][j] = d + 1;

							}
							if (j - 1 >= 0)
							{
								if (Way[i][j - 1] == -1 || Way[i][j - 1] == -6)
									Way[i][j - 1] = d + 1;

							}
							if ((j + 1) < Size.y)
							{
								if (Way[i][j + 1] == -1 || Way[i][j + 1] == -6)
									Way[i][j + 1] = d + 1;

							}
							if ((i + 1) < Size.x)
							{
								if (Way[i + 1][j] == -1 || Way[i + 1][j] == -6)
									Way[i + 1][j] = d + 1;

							}
						}
					}
				}
			}
			d++;

		} while (fl == 0 && d != Size.x * Size.y);
		if (d == Size.x * Size.y)
			return 0;
		d--;
		while (d != 1)
		{
			if (Way[x - 1][y] == d - 1)
				x--;
			else {
				if (Way[x + 1][y] == d - 1)
					x++;
				else {
					if (Way[x][y + 1] == d - 1)
						y++;
					else
						y--;
				}
			}
			d--;
		}
		if (Map[x][y].GetType() == 6) {
			Castle* C = dynamic_cast<Castle*>(Map[x][y].GetBLG());
			if (C->GetDmg(this) == 0)
				return 0;
		}
		else {
			Map[x][y].SetEnemy(Map[f.x][f.y].GetEnemy());
			Map[f.x][f.y].SetEnemy(nullptr);
			Lptr = &Map[x][y];
		}
		return 1;
	}

	int Heavy::MakeStep(Land** Map, Coord Size)
	{
		int** Way, x, y;
		Way = new int* [Size.x];
		for (int i = 0; i < Size.x; i++)
		{
			Way[i] = new int[Size.y];
			for (int j = 0; j < Size.y; j++)
			{
				int T = Map[i][j].GetType();
				if (T == 0 || T == 1 || T == 2 || T == 3)
					Way[i][j] = -1;
				else
					Way[i][j] = -2;
				if (T == 6) {
					Way[i][j] = -6;
					x = i; y = j;
				}
			}
		}
		int d = 0, fl = 0;
		Coord f = Lptr->GetCoord();
		Way[f.x][f.y] = 0;
		do {
			for (int i = 0; i < Size.x; i++)
			{
				for (int j = 0; j < Size.y; j++)
				{
					if (Way[i][j] == d)
					{
						if (i == x && j == y)
							fl = 1;
						else
						{
							if (i - 1 >= 0)
							{
								if (Way[i - 1][j] == -1 || Way[i - 1][j] == -6)
									Way[i - 1][j] = d + 1;

							}
							if (j - 1 >= 0)
							{
								if (Way[i][j - 1] == -1 || Way[i][j - 1] == -6)
									Way[i][j - 1] = d + 1;

							}
							if ((j + 1) < Size.y)
							{
								if (Way[i][j + 1] == -1 || Way[i][j + 1] == -6)
									Way[i][j + 1] = d + 1;

							}
							if ((i + 1) < Size.x)
							{
								if (Way[i + 1][j] == -1 || Way[i + 1][j] == -6)
									Way[i + 1][j] = d + 1;

							}
						}
					}
				}
			}
			d++;

		} while (fl == 0 && d != Size.x * Size.y);
		if (d == Size.x * Size.y)
			return 0;
		d--;
		while (d != 1)
		{
			if (Way[x - 1][y] == d - 1)
				x--;
			else {
				if (Way[x + 1][y] == d - 1)
					x++;
				else {
					if (Way[x][y + 1] == d - 1)
						y++;
					else
						y--;
				}
			}
			d--;
		}
		if (Map[x][y].GetType() == 3) {
			Map[x][y].SetBLG(nullptr, 0);
		}
		else {
			if (Map[x][y].GetType() == 6) {
				Castle* C = dynamic_cast<Castle*>(Map[x][y].GetBLG());
				if (C->GetDmg(this) == 0)
					return 0;
			}
			else {
				Map[x][y].SetEnemy(Map[f.x][f.y].GetEnemy());
				Map[f.x][f.y].SetEnemy(nullptr);
				Lptr = &Map[x][y];
			}
		}
		return 1;
	}

	int Plane::MakeStep(Land** Map, Coord Size)
	{
		int** Way, x, y;
		Way = new int* [Size.x];
		for (int i = 0; i < Size.x; i++)
		{
			Way[i] = new int[Size.y];
			for (int j = 0; j < Size.y; j++)
			{
				int T = Map[i][j].GetType();
				if (T == 0 || T == 1 || T == 3)
					Way[i][j] = -1;
				else
					Way[i][j] = -2;
				if (T == 6) {
					Way[i][j] = -6;
					x = i; y = j;
				}
			}
		}
		int d = 0, fl = 0;
		Coord f = Lptr->GetCoord();
		Way[f.x][f.y] = 0;
		do {
			for (int i = 0; i < Size.x; i++)
			{
				for (int j = 0; j < Size.y; j++)
				{
					if (Way[i][j] == d)
					{
						if (i == x && j == y)
							fl = 1;
						else
						{
							if (i - 1 >= 0)
							{
								if (Way[i - 1][j] == -1 || Way[i - 1][j] == -6)
									Way[i - 1][j] = d + 1;

							}
							if (j - 1 >= 0)
							{
								if (Way[i][j - 1] == -1 || Way[i][j - 1] == -6)
									Way[i][j - 1] = d + 1;

							}
							if ((j + 1) < Size.y)
							{
								if (Way[i][j + 1] == -1 || Way[i][j + 1] == -6)
									Way[i][j + 1] = d + 1;

							}
							if ((i + 1) < Size.x)
							{
								if (Way[i + 1][j] == -1 || Way[i + 1][j] == -6)
									Way[i + 1][j] = d + 1;

							}
						}
					}
				}
			}
			d++;

		} while (fl == 0 && d != Size.x * Size.y);
		if (d == Size.x * Size.y)
			return 0;
		d--;
		while (d != 1)
		{
			if (Way[x - 1][y] == d - 1)
				x--;
			else {
				if (Way[x + 1][y] == d - 1)
					x++;
				else {
					if (Way[x][y + 1] == d - 1)
						y++;
					else
						y--;
				}
			}
			d--;
		}
		if (Map[x][y].GetType() == 6) {
			Castle* C = dynamic_cast<Castle*>(Map[x][y].GetBLG());
			if (C->GetDmg(this) == 0)
				return 0;
		}
		else {
			Map[x][y].SetEnemy(Map[f.x][f.y].GetEnemy());
			Map[f.x][f.y].SetEnemy(nullptr);
			Lptr = &Map[x][y];
		}
		return 1;
	}

	void Level::Relize(Lair* L)
	{
		L->RelzTime(Map, SZ, ttime);
		ttime++;

	}

	int Level::CheckEn(Enemy* en)
	{
		for (int i = 0; i < SZ.x; i++)
			for (int j = 0; j < SZ.y; j++)
				if (Map[i][j].GetEnemy() == en)
					return 1;
		return 0;
	}
	int Level::Step(Enemy** T, Tower* T1, Tower* T2)
	{
		if (CheckEn(T[0]) == 1)
			if (T[0]->MakeStep(Map, SZ) == 0)
				return 1;
		if (CheckEn(T[1]) == 1)
			if (T[1]->MakeStep(Map, SZ) == 0)
				return 1;
		if (CheckEn(T[2]) == 1)
			if (T[2]->MakeStep(Map, SZ) == 0)
				return 1;
		T1->DmgEn(Map, SZ);
		T1->DmgEn(Map, SZ);
		if (CheckEn(T[1]) == 0 && CheckEn(T[0]) == 0 && CheckEn(T[2]) == 0)
			return -1;
		return 0;
	}
	template <class  T>
	Vector<T>::Vector(int n)
	{
		create(n);
	}

	template <class  T>
	Vector<T>::~Vector(void)
	{
		delete[] data;
	}

	template <class  T>
	int Vector<T>::Size(void) const
	{
		return size;
	}

	template <class  T>
	T& Vector<T>::operator[](int i)
	{
		return data[i];
	}

	template<class T>
	bool Vector<T>::operator==(int)
	{
		return false;
	}

	template <class T>
	Vector<T> Vector<T>::operator=(const Vector& a)
	{
		size = a.size;
		for (int i = 0; i != a.size; ++i) {
			this->date[i] = a.data[i];
		}
		return *this;
	}

	template <class T>
	bool Vector<T>::operator==(const Vector& v)
	{
		if (this->size == v.size)
		{
			for (int i = 0; i < this->size; i++)
			{
				if (this->data[i] != v.data[i])
					return false;
			}
			return true;
		}
		else {
			return  false;
		}
	}

	template <class T>
	Vector<T>::Vector(Vector& a) {
		this->size = a.size;
		for (int i = 0; i != a.size; ++i) {
			this->date[i] = a.data[i];
		}
	}

	template <class T>
	void Vector<T>::push_back(T val) {
		T* newdata;
		if (size > cap) {
			cap = cap * 2;
			newdata = new T[cap];
			for (int i = 0; i < size; i++)
				newdata[i] = data[i];
			delete[] data;
			data = newdata;
		}
		data[size] = val;
		size++;
	}

	template <class  T>
	int Vector<T>::Capacity() const
	{
		return cap;
	}

	template <class  T>
	void Vector<T>::remove(const int& i)
	{
		T* newdata = new T[cap];
		bool f = false;

		for (int j = 0; j < this->size; j++)
		{
			if (j == i)
			{
				f = true;
			}
			if (f)
			{
				newdata[j - 1] = this->data[j];
			}
			else {
				newdata[j] = this->data[j];
			}
		}
		this->size -= 1;
		delete[] this->data;
		this->data = newdata;
	}

	template <class  T>
	void Vector<T>::resize(const int& n)
	{
		T* newdata = new T[n];
		this->cap = n;
		this->size = n > this->size ? this->size : n;
		for (int i = 0; i < this->size; i++)
		{
			newdata[i] = this->data[i];
		}
		delete[] this->data;
		this->data = newdata;
	}


	template <class  T>
	void Vector<T>::clear()
	{
		delete[] this->data;
		this->size = 0;
		this->cap = 1;
		this->data = new T[1];
	}

	template <class  T>
	void Vector<T>::create(const int& i) {
		if (this->data)
			delete[] data;
		size = 0;
		cap = i;
		data = new T[i];
	}

}
