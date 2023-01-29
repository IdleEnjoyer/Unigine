// this file you need to fill
// этот файл вам нужно заполнить
#include <cmath>
#include <intrin.h>
#include "task.h"

void Task::checkVisible(const std::vector<unit> &input_units, std::vector<int> &result)
{
	std::vector<int> Amount(input_units.size(),0);
	for (std::vector<unit>::const_iterator UnitA = input_units.begin(); UnitA != input_units.end(); UnitA++) {
		float Cos = cosf((*UnitA).fov_deg * 0.008726646f);//0.008726646 = 1/2 * 1/180 * pi
		float Sin = sinf((*UnitA).fov_deg * 0.008726646f);
		float DistSquared = (*UnitA).distance * (*UnitA).distance;
		vec2 LeftSec = { (*UnitA).direction.x * Cos - (*UnitA).direction.y * Sin, (*UnitA).direction.x * Sin + (*UnitA).direction.y * Cos };
		vec2 RightSec = { (*UnitA).direction.x * Cos + (*UnitA).direction.y * Sin, (*UnitA).direction.y * Cos - (*UnitA).direction.x * Sin };
		for (std::vector<unit>::const_iterator UnitB = input_units.begin(); UnitB != input_units.end(); UnitB++) {
			vec2 VecAtoB = { (*UnitB).position.x - (*UnitA).position.x,(*UnitB).position.y - (*UnitA).position.y };

			__m128 Vecs81 = _mm_setr_ps(-LeftSec.x, LeftSec.y, -RightSec.x, RightSec.y);
			__m128 Vecs82 = _mm_setr_ps(VecAtoB.y, VecAtoB.x, VecAtoB.y, VecAtoB.x);
			Vecs81 = _mm_mul_ps(Vecs81, Vecs82);
			alignas(16) float ArrVecs4[4];
			_mm_store_ps(ArrVecs4, Vecs81);
			bool IsInDistanceA = VecAtoB.x * VecAtoB.x + VecAtoB.y * VecAtoB.y <= DistSquared;
			if (IsInDistanceA) {
				bool IsToTheRight = (ArrVecs4[0] + ArrVecs4[1] > 0);//Справа от левого сектора?
				if (IsToTheRight) {
					bool IsToTheLeft = !(ArrVecs4[2] + ArrVecs4[3] > 0);//Слева от правого сектора?
					if (IsToTheLeft) {
						Amount.at(std::distance(input_units.begin(), UnitA))++;
					}
				}
			}
		}
	}
	result = Amount;
}