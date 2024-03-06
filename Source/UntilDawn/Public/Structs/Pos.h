#pragma once

struct Pos
{
	int y, x;

	Pos(int _y, int _x) : y(_y), x(_x) {}
	Pos() : y(0), x(0) {}
	Pos operator+(Pos& _Pos)
	{
		Pos ret{ y + _Pos.y , x + _Pos.x };
		return ret;
	}
	bool operator<(const Pos& other) const
	{
		if (y != other.y)
			return y < other.y;
		return x < other.x;
	}
	bool operator!=(const Pos& other) const
	{
		return (y != other.y || x != other.x);
	}
	friend Pos operator+(const Pos& a, Pos& b)
	{
		Pos ret{ a.y + b.y, a.x + b.x };
		return ret;
	}
	friend bool operator==(const Pos& L, const Pos& R) {
		return (L.y == R.y && L.x == R.x);
	}
};