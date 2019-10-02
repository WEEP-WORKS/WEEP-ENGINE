#ifndef __VECTOR3_H__
#define __VECTOR3_H__

template<class TYPE>
class Vector3
{
public:

public:
	TYPE x, y, z;

	Vector3() {}

	Vector3(const TYPE new_x, const TYPE new_y, const TYPE new_z) : x(new_x), y(new_y), z(new_z)
	{}

	Vector3(const Vector3& new_vec)
	{
		x = new_vec.x;
		y = new_vec.y;
		z = new_vec.z;
	}

	const Vector3 normalize()
	{
		int modul = sqrtf((x * x + y * y + z * z));
		Vector3 ret;
		ret.x = x / modul;
		ret.y = y / modul;
		ret.z = z / modul;

		return ret;
	}

	void zero()
	{
		x = y = z = 0u;
	}

	bool is_zero() const
	{
		return (x == 0 && y == 0 && z == 0);
	}

	TYPE distance_to(const Vector3& target) const
	{
		TYPE final_x = target.x - x;
		TYPE final_y = target.y - y;
		TYPE final_z = target.z - z;

		return (sqrtf((final_x * final_x + final_y * final_y + final_z * final_z)));
	}
};

typedef Vector3<int> iVector3;
typedef Vector3<float> fVector3;
#endif // !__VECTOR3_H__
