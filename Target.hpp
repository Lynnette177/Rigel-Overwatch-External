#pragma once
#include "GetComponent.hpp"
#include <algorithm>
#include<complex>

namespace OW {
	inline void SetKey(uint32_t key) {
		__try {
			SDK->WPM<uint32_t>(SDK->g_player_controller + 0x1244, key);
		}
		__except (EXCEPTION_EXECUTE_HANDLER) {}
	}

	unsigned int convertToHex(ImVec4 color) {
		// 将每个分量从[0.0, 1.0]映射到[0, 255]范围，并转换为整数
		unsigned int red = static_cast<unsigned int>(color.x * 255);
		unsigned int green = static_cast<unsigned int>(color.y * 255);
		unsigned int blue = static_cast<unsigned int>(color.z * 255);
		unsigned int alpha = static_cast<unsigned int>(color.w * 255);

		// 将RGBA分量组合成一个32位整数，格式为0xAARRGGBB
		unsigned int hexColor = (alpha << 24) | (blue << 16) | (green << 8) | red;

		return hexColor;
	}

	std::pair<double, double> dd(double a, double b,
		double c, double d, double m, double n) {
		return std::pair<double, double>((m * d - c * n) / (a * d - c * b), (a * n - b * m) / (a * d - c * b));
	}

	/*inline float CalculateTrackBack(Vector3 rayOrigin, Vector3 OwnPos, Vector3 LocalAngle, Vector3 rayDirection) {
		// 计算平面法向量的模
		float rayDirectionLength = std::sqrt(rayDirection.X * rayDirection.X +
			rayDirection.Y * rayDirection.Y +
			rayDirection.Z * rayDirection.Z);

		// 确保射线方向向量的长度不为零
		if (rayDirectionLength > 1e-6) {
			// 计算点rayOrigin到平面的有向距离
			float distance = ((rayOrigin.X - OwnPos.X) * rayDirection.X +
				(rayOrigin.Y - OwnPos.Y) * rayDirection.Y +
				(rayOrigin.Z - OwnPos.Z) * rayDirection.Z) / rayDirectionLength;
			return distance/((rayDirection.X * rayDirection.X)+ (rayDirection.Y * rayDirection.Y)+ (rayDirection.Z * rayDirection.Z));
		}

		return 0; // 如果射线方向向量的长度为零，返回0表示距离无穷远
	}*/



	inline Vector3 SmoothLinear(Vector3 LocalAngle, Vector3 TargetAngle, float speed)
	{
		float dist = LocalAngle.DistTo(TargetAngle);
		static float lastx = 0;
		static float lasty = 0;
		static float lastz = 0;

		Vector3 Result;

		float deltax, deltay, deltaz;
		deltax = (TargetAngle.X - LocalAngle.X) * speed;
		deltay = (TargetAngle.Y - LocalAngle.Y) * speed;
		deltaz = (TargetAngle.Z - LocalAngle.Z) * speed;

		Result.X = LocalAngle.X + deltax;
		Result.Y = LocalAngle.Y + deltay;
		Result.Z = LocalAngle.Z + deltaz;

		if (Config::trackcompensate) {
			float radius = 1 / dist;
			Vector3 lastangle{ lastx, lasty, lastz };
			Vector3 realresult{ LocalAngle.X + TargetAngle.X - lastx ,LocalAngle.Y + TargetAngle.Y - lasty,LocalAngle.Z + TargetAngle.Z - lastz };
			if (Result.DistTo(realresult) <= Config::comarea) {
				if (abs(deltax) < abs(TargetAngle.X - lastx)) deltax = (TargetAngle.X - lastx) * Config::comspeed;
				//if (abs(deltay) < abs(TargetAngle.Y - lasty)) deltay = TargetAngle.Y - lasty;
				if (abs(deltaz) < abs(TargetAngle.Z - lastz)) deltaz = (TargetAngle.Z - lastz) * Config::comspeed;
			}
			lastx = TargetAngle.X;
			lasty = TargetAngle.Y;
			lastz = TargetAngle.Z;

			Result.X = LocalAngle.X + deltax;
			Result.Y = LocalAngle.Y + deltay;
			Result.Z = LocalAngle.Z + deltaz;
		}

		return Result;
	}

	inline void SetKeyscopeHold(int Key, float duration) {
		clock_t previous = clock();
		Key += 0x2;
		while (clock() - previous < duration) {
			SDK->WPM<uint32_t>(SDK->g_player_controller + 0x1244, Key);
			if (Config::horizonreco) {
				SDK->WPM<float>(local_entity.AngleBase + 0x1768, 0);
			}
			if (Config::norecoil) {
				SDK->WPM<float>(local_entity.AngleBase + 0x1764, Config::recoilnum);
			}
		}
		if (Config::widowautounscope) {
			while (clock() - previous < 100) {
				SDK->WPM<uint32_t>(SDK->g_player_controller + 0x1244, 0);
			}
		}
	}

	inline void SetKeyHold(int Key, float duration) {
		clock_t previous = clock();
		while (clock() - previous < duration) {
			SDK->WPM<uint32_t>(SDK->g_player_controller + 0x1244, Key);
			if (Config::horizonreco) {
				SDK->WPM<float>(local_entity.AngleBase + 0x1768, 0);
			}
			if (Config::norecoil) {
				SDK->WPM<float>(local_entity.AngleBase + 0x1764, Config::recoilnum);
			}
		}
	}

	/*inline void AimCorrection(Vector3* InVecArg, Vector3 currVelocity, float Distance, float Bulletspeed)
	{
		if (Config::hanzo_flick||local_entity.HeroID==eHero::HERO_HANJO)
		{
			float m_time = powf(Distance / Bulletspeed, 2.f);

			(*InVecArg).X = (*InVecArg).X + ((currVelocity.X) * (Distance / (Bulletspeed)));
			(*InVecArg).Y = (*InVecArg).Y + ((currVelocity.Y) * (Distance / (Bulletspeed)));
			(*InVecArg).Z = (*InVecArg).Z + ((currVelocity.Z) * (Distance / (Bulletspeed)));

			(*InVecArg).Y += (0.5f * 9.8f * m_time);
		}
		else
		{
			(*InVecArg).X = (*InVecArg).X + ((currVelocity.X) * (Distance / (Bulletspeed)));
			(*InVecArg).Y = (*InVecArg).Y + ((currVelocity.Y) * (Distance / (Bulletspeed)));
			(*InVecArg).Z = (*InVecArg).Z + ((currVelocity.Z) * (Distance / (Bulletspeed)));
		}
	}*/
	inline void SolveQuartic(const std::complex<float> coefficients[5], std::complex<float> roots[4]) {
		const std::complex<float> a = coefficients[4];
		const std::complex<float> b = coefficients[3] / a;
		const std::complex<float> c = coefficients[2] / a;
		const std::complex<float> d = coefficients[1] / a;
		const std::complex<float> e = coefficients[0] / a;

		const std::complex<float> Q1 = c * c - 3.f * b * d + 12.f * e;
		const std::complex<float> Q2 = 2.f * c * c * c - 9.f * b * c * d + 27.f * d * d + 27.f * b * b * e - 72.f * c * e;
		const std::complex<float> Q3 = 8.f * b * c - 16.f * d - 2.f * b * b * b;
		const std::complex<float> Q4 = 3.f * b * b - 8.f * c;

		const std::complex<float> Q5 = std::pow(Q2 / 2.f + std::sqrt(Q2 * Q2 / 4.f - Q1 * Q1 * Q1), 1.f / 3.f);
		const std::complex<float> Q6 = (Q1 / Q5 + Q5) / 3.f;
		const std::complex<float> Q7 = 2.f * std::sqrt(Q4 / 12.f + Q6);

		roots[0] = (-b - Q7 - std::sqrt(4.f * Q4 / 6.f - 4.f * Q6 - Q3 / Q7)) / 4.f;
		roots[1] = (-b - Q7 + std::sqrt(4.f * Q4 / 6.f - 4.f * Q6 - Q3 / Q7)) / 4.f;
		roots[2] = (-b + Q7 - std::sqrt(4.f * Q4 / 6.f - 4.f * Q6 + Q3 / Q7)) / 4.f;
		roots[3] = (-b + Q7 + std::sqrt(4.f * Q4 / 6.f - 4.f * Q6 + Q3 / Q7)) / 4.f;
	}
	inline void AimCorrection(Vector3* InVecArg, Vector3 currVelocity, float Distance, float Bulletspeed)
	{
		Vector3 Predict;
		double G = 9.8f;
		double A = viewMatrix_xor.get_location().x;
		double B = viewMatrix_xor.get_location().y;
		double C = viewMatrix_xor.get_location().z;
		double M = InVecArg->X;
		double N = InVecArg->Y;
		double O = InVecArg->Z;
		double P = currVelocity.X;
		double Q = currVelocity.Y;
		double R = currVelocity.Z;
		double S = Bulletspeed;
		double H = M - A;
		double J = O - C;
		double K = N - B;
		double L = -.5f * G;
		double c4 = L * L;
		double c3 = -2 * Q * L;
		double c2 = (Q * Q) - (2 * K * L) - (S * S) + (P * P) + (R * R);
		double c1 = (2 * K * Q) + (2 * H * P) + (2 * J * R);
		double c0 = (K * K) + (H * H) + (J * J);

		std::complex<float> pOutRoots[4];
		const std::complex<float> pInCoeffs[5] = { c0, c1, c2, c3, c4 };
		SolveQuartic(pInCoeffs, pOutRoots);
		float fBestRoot = FLT_MAX;
		for (int i = 0; i < 4; i++) {
			if (pOutRoots[i].real() > 0.f && std::abs(pOutRoots[i].imag()) < 0.0001f && pOutRoots[i].real() < fBestRoot) {
				fBestRoot = pOutRoots[i].real();
			}
		}
		InVecArg->X = viewMatrix_xor.get_location().x + (H + P * fBestRoot);
		if (Config::hanzo_flick || local_entity.HeroID == eHero::HERO_HANJO || Config::Gravitypredit) {
			InVecArg->Y = viewMatrix_xor.get_location().y + (K + Q * fBestRoot - L * fBestRoot * fBestRoot);
		}
		else InVecArg->Y += fBestRoot * currVelocity.Y;
		InVecArg->Z = viewMatrix_xor.get_location().z + (J + R * fBestRoot);
	}


	inline void AimCorrection22(Vector3* InVecArg, Vector3 currVelocity, float Distance, float Bulletspeed)
	{
		Vector3 Predict;
		double G = 9.8f;
		double A = viewMatrix_xor.get_location().x;
		double B = viewMatrix_xor.get_location().y;
		double C = viewMatrix_xor.get_location().z;
		double M = InVecArg->X;
		double N = InVecArg->Y;
		double O = InVecArg->Z;
		double P = currVelocity.X;
		double Q = currVelocity.Y;
		double R = currVelocity.Z;
		double S = Bulletspeed;
		double H = M - A;
		double J = O - C;
		double K = N - B;
		double L = -.5f * G;
		double c4 = L * L;
		double c3 = -2 * Q * L;
		double c2 = (Q * Q) - (2 * K * L) - (S * S) + (P * P) + (R * R);
		double c1 = (2 * K * Q) + (2 * H * P) + (2 * J * R);
		double c0 = (K * K) + (H * H) + (J * J);

		std::complex<float> pOutRoots[4];
		const std::complex<float> pInCoeffs[5] = { c0, c1, c2, c3, c4 };
		SolveQuartic(pInCoeffs, pOutRoots);
		float fBestRoot = FLT_MAX;
		for (int i = 0; i < 4; i++) {
			if (pOutRoots[i].real() > 0.f && std::abs(pOutRoots[i].imag()) < 0.0001f && pOutRoots[i].real() < fBestRoot) {
				fBestRoot = pOutRoots[i].real();
			}
		}
		InVecArg->X = viewMatrix_xor.get_location().x + (H + P * fBestRoot);
		if (Config::Gravitypredit2) {
			InVecArg->Y = viewMatrix_xor.get_location().y + (K + Q * fBestRoot - L * fBestRoot * fBestRoot);
		}
		else InVecArg->Y += fBestRoot * currVelocity.Y;
		InVecArg->Z = viewMatrix_xor.get_location().z + (J + R * fBestRoot);
	}

	inline bool in_range(Vector3 MyAngle, Vector3 EnemyAngle, Vector3 MyPosition, Vector3 EnemyPosition, float radius)
	{
		float dist = MyPosition.DistTo(EnemyPosition);
		radius /= dist;
		Vector3 sub = MyAngle - EnemyAngle;

		return MyAngle.DistTo(EnemyAngle) <= radius;
	}

	inline Vector3 SmoothAccelerate(Vector3 LocalAngle, Vector3 TargetAngle, float Speed, float Acc)
	{
		Vector3 Result = LocalAngle;
		__try
		{
			Vector3 delta = TargetAngle - LocalAngle;

			float tmp = Acc / delta.get_length();
			tmp = tmp * tmp * 0.005f;

			float c = Speed;
			float chu = tmp * Speed;
			c += chu;

			if (c >= 1) c = 1;

			Result.X += delta.X * c;
			Result.Y += delta.Y * c;
			Result.Z += delta.Z * c;

			return Result;
		}
		__except (1)
		{
			return Result;
		}
	}

	inline XMFLOAT3 CalcAngle(XMFLOAT3 Target, XMFLOAT3 CameraPos) {
		XMFLOAT3 Result;
		float Distance = XMVectorGetX(XMVector3Length(XMLoadFloat3(&Target) - XMLoadFloat3(&CameraPos)));
		XMStoreFloat3(&Result, (XMLoadFloat3(&Target) - XMLoadFloat3(&CameraPos)) / Distance);
		return Result;
	}


	float GetLookUpSkill(uint16_t a1) // for get Hanzo's Projectile Speed
	{
		__try
		{
			uint64_t pSkill = SDK->RPM<uint64_t>(local_entity.SkillBase + 0x1848);
			uint64_t SkillRawList = SDK->RPM<uint64_t>(pSkill + 0x10);
			uint32_t SkillSize = SDK->RPM<uint32_t>(pSkill + 0x18);
			for (uint32_t i = 0x0; i < SkillSize; i++)
			{
				if (SDK->RPM<uint16_t>(SkillRawList + (i * 0x80)) == a1)
					if (SDK->RPM<float>(SkillRawList + (i * 0x80) + 0x30) >= 0.5f) // 50% 捞惑 瞒隆 矫 
						return SDK->RPM<float>(SkillRawList + (i * 0x80) + 0x30);
			}
		}
		__except (1) {}
		return 0.f;

	}

	Vector3 GetVector3(bool predit = false)
	{
		int TarGetIndex = -1;
		Vector3 target{};
		Vector2 CrossHair = Vector2(GetSystemMetrics(SM_CXSCREEN) / 2.0f, GetSystemMetrics(SM_CYSCREEN) / 2.0f);

		float origin = 100000.f;
		float dist = 100000.f;
		//if (TarGetIndex == -1)
		//{
		if (entities.size() > 0) {
			{
				if (Config::hanzoautospeed) {
					if (local_entity.HeroID == eHero::HERO_HANJO) {
						Config::predit_level = readult(local_entity.SkillBase + 0x50, 0xC, 0x2A5) * 85.f + 25.f;
						if (local_entity.skill2act) Config::predit_level = 110.f;
					}
				}
				Vector3 Vel;
				Vector3 PreditPos;
				Vector3 RootPos;
				for (int i = 0; i < entities.size(); i++)
				{
					//if (entities[i].HeroID == 0x16dd) continue;
					if (!Config::switch_team) {
						if (entities[i].Alive && entities[i].Team && entities[i].Vis)
						{
							if (Config::Bone == 1) {
								PreditPos = entities[i].head_pos;
								RootPos = entities[i].head_pos;
							}
							else if (Config::Bone == 2) {
								PreditPos = entities[i].neck_pos;
								RootPos = entities[i].neck_pos;
							}
							else if (Config::Bone == 3) {
								PreditPos = entities[i].chest_pos;
								RootPos = entities[i].chest_pos;
							}
							/*if (Config::autobone) {
								Vector3 RootPos1;
								Vector3 RootPos2;
								Vector3 RootPos3;
								RootPos1 = entities[i].head_pos;
								Vector2 POS1 = viewMatrix.WorldToScreen(RootPos1);
								RootPos2 = entities[i].GetBonePos(entities[i].GetSkel()[16]);
								Vector2 POS2 = viewMatrix.WorldToScreen(RootPos2);
								RootPos3 = entities[i].chest_pos;
								Vector2 POS3 = viewMatrix.WorldToScreen(RootPos3);
								float CrossDistpos1 = CrossHair.Distance(POS1);
								float CrossDistpos2 = CrossHair.Distance(POS2);
								float CrossDistpos3 = CrossHair.Distance(POS3);
								if (CrossDistpos1 < CrossDistpos2 && CrossDistpos1 < CrossDistpos3) RootPos = RootPos1;
								else if (CrossDistpos2 < CrossDistpos1 && CrossDistpos2 < CrossDistpos3) RootPos = RootPos2;
								else if (CrossDistpos3 < CrossDistpos1 && CrossDistpos3 < CrossDistpos2) RootPos = RootPos3;
								PreditPos = RootPos;
							}*/
							/*if (Config::autobone) {
								if (entities[i].HeroID == eHero::HERO_TRAININGBOT1 || entities[i].HeroID == eHero::HERO_TRAININGBOT2 || entities[i].HeroID == eHero::HERO_TRAININGBOT3 || entities[i].HeroID == eHero::HERO_TRAININGBOT4 || entities[i].HeroID == eHero::HERO_TRAININGBOT5 || entities[i].HeroID == eHero::HERO_TRAININGBOT6 || entities[i].HeroID == eHero::HERO_TRAININGBOT7) {
									float distbone[5] = { 0 };
									int index[] = { 17,16,3,13,54 };
									Vector3 bonerootpos{};
									Vector2 bonecrosspos{};
									for (int iii = 0; iii < 5; iii++) {
										bonerootpos= entities[i].GetBonePos(index[iii]);
										if (bonerootpos == entities[i].pos) distbone[iii] = 100000;
										else {
											bonecrosspos = viewMatrix.WorldToScreen(bonerootpos);
											distbone[iii] = CrossHair.Distance(bonecrosspos);
										}
									}
									int m;
									m = std::min_element(distbone, distbone + 5) - distbone;
									RootPos = entities[i].GetBonePos(index[m]);
									PreditPos = RootPos;
								}
								else {
									float distbone[18] = { 0 };
									Vector3 bonerootpos{};
									Vector2 bonecrosspos{};
									for (int iii = 0; iii < 18; iii++) {
										bonerootpos = entities[i].GetBonePos(entities[i].GetSkel()[iii]);
										bonecrosspos = viewMatrix.WorldToScreen(bonerootpos);
										distbone[iii] = CrossHair.Distance(bonecrosspos);
									}
									int m;
									m = std::min_element(distbone, distbone + 18) - distbone;
									RootPos = entities[i].GetBonePos(entities[i].GetSkel()[m]);
									PreditPos = RootPos;
								}
							}*/
							Vel = entities[i].velocity;
							/*if (entities[i].HeroID == eHero::HERO_WRECKINGBALL) {
								PreditPos.Y -= 0.7;
								RootPos.Y -= 0.7;
							}*/
							if (predit)
							{
								dist = Vector3(viewMatrix_xor.get_location().x, viewMatrix_xor.get_location().y, viewMatrix_xor.get_location().z).DistTo(PreditPos);
								//if (local_entity.HeroID == eHero::HERO_HANJO && GetLookUpSkill(0xC9) >= 0.5f)
								//{
								//	AimCorrection(&PreditPos, Vel, dist, 25.f + (85.f * GetLookUpSkill(0xC9)));
								//}
								//else
								AimCorrection(&PreditPos, Vel, dist, Config::predit_level);
							}
							Vector2 Vec2 = predit ? viewMatrix.WorldToScreen(PreditPos) : viewMatrix.WorldToScreen(RootPos);
							float CrossDist = CrossHair.Distance(Vec2);
							if (CrossDist < origin && CrossDist <= Config::Fov && !Config::fov360)
							{
								target = predit ? PreditPos : RootPos;
								origin = CrossDist;
								TarGetIndex = i;
								/*Config::health = entities[i].PlayerHealth;
								if (GetAsyncKeyState(Config::aim_key)) {
									Config::Targetenemyi = i;
									Config::targetenemy = convertToHex(Config::targetargb);
									SDK->WPM<uint32_t>(entities[i].OutlineBase + 0x144, Config::targetenemy);
									SDK->WPM<uint32_t>(entities[i].OutlineBase + 0x130, Config::targetenemy);
								}*/
							}
							else if (Config::fov360) {
								CrossDist = Vector3(viewMatrix_xor.get_location().x, viewMatrix_xor.get_location().y, viewMatrix_xor.get_location().z).DistTo(PreditPos);
								if (CrossDist < origin)
								{
									target = predit ? PreditPos : RootPos;
									origin = CrossDist;
									TarGetIndex = i;
								}
							}
							//else
								//TarGetIndex = -1;
						}
						//else
							//TarGetIndex = -1;
					}
					else if (Config::switch_team) {
						if (entities[i].Alive && !entities[i].Team && entities[i].Vis && entities[i].address != local_entity.address)
						{
							/*Config::health = entities[i].PlayerHealth;
							if (GetAsyncKeyState(Config::aim_key)) {
								Config::Targetenemyi = i;
								Config::targetenemy = convertToHex(Config::targetargb);
								SDK->WPM<uint32_t>(entities[i].OutlineBase + 0x144, Config::targetenemy);
								SDK->WPM<uint32_t>(entities[i].OutlineBase + 0x130, Config::targetenemy);
							}*/
							if (Config::Bone == 1) {
								PreditPos = entities[i].head_pos;
								RootPos = entities[i].head_pos;
							}
							else if (Config::Bone == 2) {
								PreditPos = entities[i].neck_pos;
								RootPos = entities[i].neck_pos;
							}
							else if (Config::Bone == 3) {
								PreditPos = entities[i].chest_pos;
								RootPos = entities[i].chest_pos;
							}
							Vel = entities[i].velocity;
							/*if (entities[i].HeroID == eHero::HERO_WRECKINGBALL) {
								PreditPos.Y -= 0.7;
								RootPos.Y -= 0.7;
							}*/
							if (predit)
							{
								float dist = Vector3(viewMatrix_xor.get_location().x, viewMatrix_xor.get_location().y, viewMatrix_xor.get_location().z).DistTo(PreditPos);
								//if (local_entity.HeroID == eHero::HERO_HANJO && GetLookUpSkill(0xC9) >= 0.5f)
								//{
								//	AimCorrection(&PreditPos, Vel, dist, 25.f + (85.f * GetLookUpSkill(0xC9)));
								//}
								//else
								AimCorrection(&PreditPos, Vel, dist, Config::predit_level);
							}
							Vector2 Vec2 = predit ? viewMatrix.WorldToScreen(PreditPos) : viewMatrix.WorldToScreen(RootPos);
							float CrossDist = CrossHair.Distance(Vec2);

							if (CrossDist < origin && CrossDist <= Config::Fov && !Config::fov360)
							{
								target = predit ? PreditPos : RootPos;
								origin = CrossDist;
								TarGetIndex = i;
								/*Config::health = entities[i].PlayerHealth;
								if (GetAsyncKeyState(Config::aim_key)) {
									Config::Targetenemyi = i;
									Config::targetenemy = convertToHex(Config::targetargb);
									SDK->WPM<uint32_t>(entities[i].OutlineBase + 0x144, Config::targetenemy);
									SDK->WPM<uint32_t>(entities[i].OutlineBase + 0x130, Config::targetenemy);
								}*/
							}
							else if (Config::fov360) {
								CrossDist = Vector3(viewMatrix_xor.get_location().x, viewMatrix_xor.get_location().y, viewMatrix_xor.get_location().z).DistTo(PreditPos);
								if (CrossDist < origin)
								{
									target = predit ? PreditPos : RootPos;
									origin = CrossDist;
									TarGetIndex = i;
								}
							}
							//else
								//TarGetIndex = -1;
						}
						//else
							//TarGetIndex = -1;
					}
				}
				if (TarGetIndex != -1) {
					Config::health = entities[TarGetIndex].PlayerHealth;
					Config::Targetenemyi = TarGetIndex;
					if (Config::autobone && entities[TarGetIndex].HeroID != 0x16dd && entities[TarGetIndex].HeroID != 0x16ee) {
						if (entities[TarGetIndex].HeroID == eHero::HERO_TRAININGBOT1 || entities[TarGetIndex].HeroID == eHero::HERO_TRAININGBOT2 || entities[TarGetIndex].HeroID == eHero::HERO_TRAININGBOT3 || entities[TarGetIndex].HeroID == eHero::HERO_TRAININGBOT4 || entities[TarGetIndex].HeroID == eHero::HERO_TRAININGBOT5 || entities[TarGetIndex].HeroID == eHero::HERO_TRAININGBOT6 || entities[TarGetIndex].HeroID == eHero::HERO_TRAININGBOT7) {
							float distbone[5] = { 0 };
							int index[] = { 17,16,3,13,54 };
							Vector3 bonerootpos{};
							Vector2 bonecrosspos{};
							for (int iii = 0; iii < 5; iii++) {
								bonerootpos = entities[TarGetIndex].GetBonePos(index[iii]);
								if (bonerootpos == entities[TarGetIndex].pos) distbone[iii] = 100000;
								else {
									bonecrosspos = viewMatrix.WorldToScreen(bonerootpos);
									distbone[iii] = CrossHair.Distance(bonecrosspos);
								}
							}
							int m;
							m = std::min_element(distbone, distbone + 5) - distbone;
							RootPos = entities[TarGetIndex].GetBonePos(index[m]);
							PreditPos = RootPos;
							target = RootPos;
							if (predit)
							{
								float dist = Vector3(viewMatrix_xor.get_location().x, viewMatrix_xor.get_location().y, viewMatrix_xor.get_location().z).DistTo(PreditPos);
								//if (local_entity.HeroID == eHero::HERO_HANJO && GetLookUpSkill(0xC9) >= 0.5f)
								//{
								//	AimCorrection(&PreditPos, Vel, dist, 25.f + (85.f * GetLookUpSkill(0xC9)));
								//}
								//else
								Vel = entities[TarGetIndex].velocity;
								AimCorrection(&PreditPos, Vel, dist, Config::predit_level);
								target = PreditPos;
							}
						}
						else {
							float distbone[12] = { 0 };
							Vector3 bonerootpos{};
							Vector2 bonecrosspos{};
							for (int iii = 0; iii < 12; iii++) {
								bonerootpos = entities[TarGetIndex].GetBonePos(entities[TarGetIndex].GetSkel()[iii]);
								bonecrosspos = viewMatrix.WorldToScreen(bonerootpos);
								distbone[iii] = CrossHair.Distance(bonecrosspos);
							}
							int m;
							m = std::min_element(distbone, distbone + 12) - distbone;
							RootPos = entities[TarGetIndex].GetBonePos(entities[TarGetIndex].GetSkel()[m]);
							PreditPos = RootPos;
							target = RootPos;
							if (predit)
							{
								float dist = Vector3(viewMatrix_xor.get_location().x, viewMatrix_xor.get_location().y, viewMatrix_xor.get_location().z).DistTo(PreditPos);
								//if (local_entity.HeroID == eHero::HERO_HANJO && GetLookUpSkill(0xC9) >= 0.5f)
								//{
								//	AimCorrection(&PreditPos, Vel, dist, 25.f + (85.f * GetLookUpSkill(0xC9)));
								//}
								//else
								Vel = entities[TarGetIndex].velocity;
								AimCorrection(&PreditPos, Vel, dist, Config::predit_level);
								target = PreditPos;
							}
						}
					}
					if (GetAsyncKeyState(Config::aim_key) && Config::externaloutline) {
						Config::targetenemy = convertToHex(Config::targetargb);
						SDK->WPM<uint32_t>(entities[TarGetIndex].OutlineBase + 0x144, Config::targetenemy);
						SDK->WPM<uint32_t>(entities[TarGetIndex].OutlineBase + 0x130, Config::targetenemy);
					}
				}
			}
		}

		//else
		/* {
			if (entities[TarGetIndex].Alive && entities[TarGetIndex].Team && entities[TarGetIndex].Vis)
			{
				//Config::health = entities[TarGetIndex].PlayerHealth;
				//if (GetAsyncKeyState(Config::aim_key)) {
				//	Config::Targetenemyi = TarGetIndex;
				//	Config::targetenemy = convertToHex(Config::targetargb);
				//	SDK->WPM<uint32_t>(entities[TarGetIndex].OutlineBase + 0x144, Config::targetenemy);
				//	SDK->WPM<uint32_t>(entities[TarGetIndex].OutlineBase + 0x130, Config::targetenemy);
				//}
				Vector3 PreditPos;
				Vector3 RootPos;
				if (Config::Bone == 1) {
					PreditPos = entities[TarGetIndex].head_pos;
					RootPos = entities[TarGetIndex].head_pos;
				}
				else if (Config::Bone == 2) {
					PreditPos = entities[TarGetIndex].neck_pos;
					RootPos = entities[TarGetIndex].neck_pos;
				}
				else if (Config::Bone == 3) {
					PreditPos = entities[TarGetIndex].chest_pos;
					RootPos = entities[TarGetIndex].chest_pos;
				}
				if (Config::autobone) {
					Vector3 RootPos1;
					Vector3 RootPos2;
					Vector3 RootPos3;
					RootPos1 = entities[TarGetIndex].head_pos;
					Vector2 POS1 = viewMatrix.WorldToScreen(RootPos1);
					RootPos2 = entities[TarGetIndex].GetBonePos(entities[TarGetIndex].GetSkel()[16]);
					Vector2 POS2 = viewMatrix.WorldToScreen(RootPos2);
					RootPos3 = entities[TarGetIndex].chest_pos;
					Vector2 POS3 = viewMatrix.WorldToScreen(RootPos3);
					float CrossDistpos1 = CrossHair.Distance(POS1);
					float CrossDistpos2 = CrossHair.Distance(POS2);
					float CrossDistpos3 = CrossHair.Distance(POS3);
					if (CrossDistpos1 < CrossDistpos2 && CrossDistpos1 < CrossDistpos3) RootPos = RootPos1;
					else if (CrossDistpos2 < CrossDistpos1 && CrossDistpos2 < CrossDistpos3) RootPos = RootPos2;
					else if (CrossDistpos3 < CrossDistpos1 && CrossDistpos3 < CrossDistpos2) RootPos = RootPos3;
					PreditPos = RootPos;
				}
				Vector3 Vel = entities[TarGetIndex].velocity;
				if (entities[TarGetIndex].HeroID == eHero::HERO_WRECKINGBALL) {
					PreditPos.Y -= 0.7;
					RootPos.Y -= 0.7;
				}
				if (predit)
				{
					float dist = Vector3(viewMatrix_xor.get_location().x, viewMatrix_xor.get_location().y, viewMatrix_xor.get_location().z).DistTo(PreditPos);
					//if (local_entity.HeroID == eHero::HERO_HANJO && GetLookUpSkill(0xC9) >= 0.5f)
					//{
					//	AimCorrection(&PreditPos, Vel, dist, 25.f + (85.f * GetLookUpSkill(0xC9)));
					//}
					//else
					AimCorrection(&PreditPos, Vel, dist, Config::predit_level);
				}
				Vector2 Vec2 = predit ? viewMatrix.WorldToScreen(PreditPos) : viewMatrix.WorldToScreen(RootPos);
				float CrossDist = CrossHair.Distance(Vec2);

				if (CrossDist < origin && CrossDist <= Config::Fov)
				{
					target = predit ? PreditPos : RootPos;
					origin = CrossDist;
					Config::health = entities[TarGetIndex].PlayerHealth;
					if (GetAsyncKeyState(Config::aim_key)) {
						Config::Targetenemyi = TarGetIndex;
						Config::targetenemy = convertToHex(Config::targetargb);
						SDK->WPM<uint32_t>(entities[TarGetIndex].OutlineBase + 0x144, Config::targetenemy);
						SDK->WPM<uint32_t>(entities[TarGetIndex].OutlineBase + 0x130, Config::targetenemy);
					}
				}
				else
				{
					TarGetIndex = -1;

				}
			}
			else
			{
				TarGetIndex = -1;
			}
		}*/
		if (local_entity.HeroID == eHero::HERO_ASHE) {
			for (hpanddy hppack : hp_dy_entities) {
				if (hppack.entityid == 0x400000000002533) {
					Vector2 Vec2 = viewMatrix.WorldToScreen(Vector3(hppack.POS.x, hppack.POS.y, hppack.POS.z));
					float CrossDist = CrossHair.Distance(Vec2);
					if (CrossDist < origin && CrossDist <= Config::Fov) {
						target = Vector3(hppack.POS.x, hppack.POS.y, hppack.POS.z);
					}
					else if (Config::fov360) target = Vector3(hppack.POS.x, hppack.POS.y, hppack.POS.z);
					break;
				}
			}
		}
		return target;
		//}
	}
	Vector3 GetVector3fortrackback(bool predit = false)
	{
		int TarGetIndex = -1;
		Vector3 target{};
		if (local_entity.HeroID == eHero::HERO_HANJO) predit = true;
		Vector2 CrossHair = Vector2(GetSystemMetrics(SM_CXSCREEN) / 2.0f, GetSystemMetrics(SM_CYSCREEN) / 2.0f);

		float origin = 100000.f;
		if (entities.size() > 0) {
			{
				Vector3 PreditPos;
				Vector3 RootPos;
				Vector3 Vel;
				for (int i = 0; i < entities.size(); i++)
				{
					//if (entities[i].HeroID == 0x16dd) continue;
					if (!Config::switch_team) {
						if (entities[i].Alive && entities[i].Team)
						{
							if (Config::Bone == 1) {
								PreditPos = entities[i].head_pos;
								RootPos = entities[i].head_pos;
							}
							else if (Config::Bone == 2) {
								PreditPos = entities[i].neck_pos;
								RootPos = entities[i].neck_pos;
							}
							else if (Config::Bone == 3) {
								PreditPos = entities[i].chest_pos;
								RootPos = entities[i].chest_pos;
							}
							Vel = entities[i].velocity;
							/*if (entities[i].HeroID == eHero::HERO_WRECKINGBALL) {
								PreditPos.Y -= 0.7;
								RootPos.Y -= 0.7;
							}*/
							if (predit)
							{
								float dist = Vector3(viewMatrix_xor.get_location().x, viewMatrix_xor.get_location().y, viewMatrix_xor.get_location().z).DistTo(PreditPos);
								AimCorrection(&PreditPos, Vel, dist, Config::predit_level);
							}
							Vector2 Vec2 = predit ? viewMatrix.WorldToScreen(PreditPos) : viewMatrix.WorldToScreen(RootPos);
							float CrossDist = CrossHair.Distance(Vec2);

							if (CrossDist < origin && CrossDist <= Config::Fov)
							{
								target = predit ? PreditPos : RootPos;
								origin = CrossDist;
								TarGetIndex = i;
							}
						}
					}
					else if (Config::switch_team) {
						if (entities[i].Alive && !entities[i].Team && entities[i].Vis && entities[i].address != local_entity.address)
						{
							if (Config::Bone == 1) {
								PreditPos = entities[i].head_pos;
								RootPos = entities[i].head_pos;
							}
							else if (Config::Bone == 2) {
								PreditPos = entities[i].neck_pos;
								RootPos = entities[i].neck_pos;
							}
							else if (Config::Bone == 3) {
								PreditPos = entities[i].chest_pos;
								RootPos = entities[i].chest_pos;
							}
							Vector3 Vel = entities[i].velocity;
							/*if (entities[i].HeroID == eHero::HERO_WRECKINGBALL) {
								PreditPos.Y -= 0.7;
								RootPos.Y -= 0.7;
							}*/
							if (predit)
							{
								float dist = Vector3(viewMatrix_xor.get_location().x, viewMatrix_xor.get_location().y, viewMatrix_xor.get_location().z).DistTo(PreditPos);
								AimCorrection(&PreditPos, Vel, dist, Config::predit_level);
							}
							Vector2 Vec2 = predit ? viewMatrix.WorldToScreen(PreditPos) : viewMatrix.WorldToScreen(RootPos);
							float CrossDist = CrossHair.Distance(Vec2);

							if (CrossDist < origin && CrossDist <= Config::Fov && !Config::fov360)
							{
								target = predit ? PreditPos : RootPos;
								origin = CrossDist;
								TarGetIndex = i;
								/*Config::health = entities[i].PlayerHealth;
								if (GetAsyncKeyState(Config::aim_key)) {
									Config::Targetenemyi = i;
									Config::targetenemy = convertToHex(Config::targetargb);
									SDK->WPM<uint32_t>(entities[i].OutlineBase + 0x144, Config::targetenemy);
									SDK->WPM<uint32_t>(entities[i].OutlineBase + 0x130, Config::targetenemy);
								}*/
							}
							else if (Config::fov360) {
								CrossDist = Vector3(viewMatrix_xor.get_location().x, viewMatrix_xor.get_location().y, viewMatrix_xor.get_location().z).DistTo(PreditPos);
								if (CrossDist < origin)
								{
									target = predit ? PreditPos : RootPos;
									origin = CrossDist;
									TarGetIndex = i;
								}
							}
						}
					}
				}
				if (TarGetIndex != -1) {
					if ((Config::autobone || Config::autobone2) && entities[TarGetIndex].HeroID != 0x16dd && entities[TarGetIndex].HeroID != 0x16ee) {
						if (entities[TarGetIndex].HeroID == eHero::HERO_TRAININGBOT1 || entities[TarGetIndex].HeroID == eHero::HERO_TRAININGBOT2 || entities[TarGetIndex].HeroID == eHero::HERO_TRAININGBOT3 || entities[TarGetIndex].HeroID == eHero::HERO_TRAININGBOT4 || entities[TarGetIndex].HeroID == eHero::HERO_TRAININGBOT5 || entities[TarGetIndex].HeroID == eHero::HERO_TRAININGBOT6 || entities[TarGetIndex].HeroID == eHero::HERO_TRAININGBOT7) {
							float distbone[5] = { 0 };
							int index[] = { 17,16,3,13,54 };
							Vector3 bonerootpos{};
							Vector2 bonecrosspos{};
							for (int iii = 0; iii < 5; iii++) {
								bonerootpos = entities[TarGetIndex].GetBonePos(index[iii]);
								if (bonerootpos == entities[TarGetIndex].pos) distbone[iii] = 100000;
								else {
									bonecrosspos = viewMatrix.WorldToScreen(bonerootpos);
									distbone[iii] = CrossHair.Distance(bonecrosspos);
								}
							}
							int m;
							m = std::min_element(distbone, distbone + 5) - distbone;
							RootPos = entities[TarGetIndex].GetBonePos(index[m]);
							PreditPos = RootPos;
							target = RootPos;
							if (predit)
							{
								float dist = Vector3(viewMatrix_xor.get_location().x, viewMatrix_xor.get_location().y, viewMatrix_xor.get_location().z).DistTo(PreditPos);
								//if (local_entity.HeroID == eHero::HERO_HANJO && GetLookUpSkill(0xC9) >= 0.5f)
								//{
								//	AimCorrection(&PreditPos, Vel, dist, 25.f + (85.f * GetLookUpSkill(0xC9)));
								//}
								//else
								Vel = entities[TarGetIndex].velocity;
								AimCorrection(&PreditPos, Vel, dist, Config::predit_level);
								target = PreditPos;
							}
						}
						else {
							float distbone[12] = { 0 };
							Vector3 bonerootpos{};
							Vector2 bonecrosspos{};
							for (int iii = 0; iii < 12; iii++) {
								bonerootpos = entities[TarGetIndex].GetBonePos(entities[TarGetIndex].GetSkel()[iii]);
								bonecrosspos = viewMatrix.WorldToScreen(bonerootpos);
								distbone[iii] = CrossHair.Distance(bonecrosspos);
							}
							int m;
							m = std::min_element(distbone, distbone + 12) - distbone;
							RootPos = entities[TarGetIndex].GetBonePos(entities[TarGetIndex].GetSkel()[m]);
							PreditPos = RootPos;
							target = RootPos;
							if (predit)
							{
								Vel = entities[TarGetIndex].velocity;
								float dist = Vector3(viewMatrix_xor.get_location().x, viewMatrix_xor.get_location().y, viewMatrix_xor.get_location().z).DistTo(PreditPos);
								AimCorrection(&PreditPos, Vel, dist, Config::predit_level);
								target = PreditPos;
							}
						}
					}
					Config::health = entities[TarGetIndex].PlayerHealth;
				}
			}
		}

		return target;
	}

	Vector3 GetVector3forgenji(bool predit = false)
	{
		int TarGetIndex = -1;
		Vector3 target{};
		Vector2 CrossHair = Vector2(GetSystemMetrics(SM_CXSCREEN) / 2.0f, GetSystemMetrics(SM_CYSCREEN) / 2.0f);

		float origin = 100000.f;
		float dist = 100000.f;
		if (entities.size() > 0) {
			{
				for (int i = 0; i < entities.size(); i++)
				{
					if (entities[i].HeroID == 0x16dd || entities[i].HeroID == 0x16ee) continue;
					if (entities[i].HeroID == eHero::HERO_GENJI && entities[i].skill2act) continue;
					if (!Config::switch_team) {
						if (entities[i].Alive && entities[i].Team && entities[i].Vis)
						{

							Vector3 PreditPos;
							Vector3 RootPos;
							if (!local_entity.skillcd1)
								PreditPos = entities[i].GetBonePos(entities[i].GetSkel()[16]);
							else PreditPos = entities[i].GetBonePos(entities[i].GetSkel()[2]);
							if (entities[i].HeroID == eHero::HERO_TRAININGBOT1 || entities[i].HeroID == eHero::HERO_TRAININGBOT2 || entities[i].HeroID == eHero::HERO_TRAININGBOT3 || entities[i].HeroID == eHero::HERO_TRAININGBOT4 || entities[i].HeroID == eHero::HERO_TRAININGBOT5 || entities[i].HeroID == eHero::HERO_TRAININGBOT6 || entities[i].HeroID == eHero::HERO_TRAININGBOT7) {
								PreditPos = entities[i].GetBonePos(3);
								if (!local_entity.skillcd1) PreditPos.Y -= 0.4;
							}
							RootPos = PreditPos;
							Vector3 Vel = entities[i].velocity;
							if (entities[i].HeroID == eHero::HERO_WRECKINGBALL) {
								PreditPos.Y -= 0.7;
								RootPos.Y -= 0.7;
							}
							Vector2 Vec2 = predit ? viewMatrix.WorldToScreen(PreditPos) : viewMatrix.WorldToScreen(RootPos);
							float CrossDist;
							CrossDist = Vector3(viewMatrix_xor.get_location().x, viewMatrix_xor.get_location().y, viewMatrix_xor.get_location().z).DistTo(PreditPos);
							if (entities[i].PlayerHealth > 200.f) CrossDist = entities[i].PlayerHealth;
							if (entities[i].HeroID == eHero::HERO_ZENYATTA && entities[i].ultimate == 0.f) CrossDist = 1000;
							if (CrossDist < origin && CrossDist <= 6000)
							{
								target = RootPos;
								origin = CrossDist;
								TarGetIndex = i;
							}
						}
					}
				}
				if (TarGetIndex != -1) {
					Config::health = entities[TarGetIndex].PlayerHealth;
					Config::Targetenemyi = TarGetIndex;
					if (Config::externaloutline) {
						Config::targetenemy = convertToHex(Config::targetargb);
						SDK->WPM<uint32_t>(entities[TarGetIndex].OutlineBase + 0x144, Config::targetenemy);
						SDK->WPM<uint32_t>(entities[TarGetIndex].OutlineBase + 0x130, Config::targetenemy);
					}
				}
			}
		}
		return target;
	}


	Vector3 GetVector3aim2(bool predit = false)
	{
		int TarGetIndex = -1;
		Vector3 target{};
		Vector2 CrossHair = Vector2(GetSystemMetrics(SM_CXSCREEN) / 2.0f, GetSystemMetrics(SM_CYSCREEN) / 2.0f);
		Vector3 PreditPos;
		Vector3 RootPos;
		float origin = 100000.f;
		float dist = 100000.f;
		Vector3 Vel;
		if (entities.size() > 0) {
			{
				for (int i = 0; i < entities.size(); i++)
				{

					if (!Config::switch_team2) {
						if (entities[i].Alive && entities[i].Team && entities[i].Vis)
						{
							if (Config::Bone2 == 1) {
								PreditPos = entities[i].head_pos;
								RootPos = entities[i].head_pos;
							}
							else if (Config::Bone2 == 2) {
								PreditPos = entities[i].neck_pos;
								RootPos = entities[i].neck_pos;
							}
							else if (Config::Bone2 == 3) {
								PreditPos = entities[i].chest_pos;
								RootPos = entities[i].chest_pos;
							}
							Vel = entities[i].velocity;
							/*if (entities[i].HeroID == eHero::HERO_WRECKINGBALL) {
								PreditPos.Y -= 0.7;
								RootPos.Y -= 0.7;
							}*/
							if (predit)
							{
								dist = Vector3(viewMatrix_xor.get_location().x, viewMatrix_xor.get_location().y, viewMatrix_xor.get_location().z).DistTo(PreditPos);
								AimCorrection22(&PreditPos, Vel, dist, Config::predit_level2);
							}
							Vector2 Vec2 = predit ? viewMatrix.WorldToScreen(PreditPos) : viewMatrix.WorldToScreen(RootPos);
							float CrossDist = CrossHair.Distance(Vec2);
							if (Config::fov360) CrossDist = Vector3(viewMatrix_xor.get_location().x, viewMatrix_xor.get_location().y, viewMatrix_xor.get_location().z).DistTo(PreditPos);;

							if (CrossDist < origin && CrossDist <= Config::Fov && !Config::fov360)
							{
								target = predit ? PreditPos : RootPos;
								origin = CrossDist;
								TarGetIndex = i;
								/*Config::health = entities[i].PlayerHealth;
								if (GetAsyncKeyState(Config::aim_key)) {
									Config::Targetenemyi = i;
									Config::targetenemy = convertToHex(Config::targetargb);
									SDK->WPM<uint32_t>(entities[i].OutlineBase + 0x144, Config::targetenemy);
									SDK->WPM<uint32_t>(entities[i].OutlineBase + 0x130, Config::targetenemy);
								}*/
							}
							else if (Config::fov360) {
								CrossDist = Vector3(viewMatrix_xor.get_location().x, viewMatrix_xor.get_location().y, viewMatrix_xor.get_location().z).DistTo(PreditPos);
								if (CrossDist < origin)
								{
									target = predit ? PreditPos : RootPos;
									origin = CrossDist;
									TarGetIndex = i;
								}
							}

						}

					}
					else if (Config::switch_team2) {
						if (entities[i].Alive && !entities[i].Team && entities[i].Vis && entities[i].address != local_entity.address)
						{
							if (Config::Bone == 1) {
								PreditPos = entities[i].head_pos;
								RootPos = entities[i].head_pos;
							}
							else if (Config::Bone == 2) {
								PreditPos = entities[i].neck_pos;
								RootPos = entities[i].neck_pos;
							}
							else if (Config::Bone == 3) {
								PreditPos = entities[i].chest_pos;
								RootPos = entities[i].chest_pos;
							}
							Vel = entities[i].velocity;
							/*if (entities[i].HeroID == eHero::HERO_WRECKINGBALL) {
								PreditPos.Y -= 0.7;
								RootPos.Y -= 0.7;
							}*/
							if (predit)
							{
								float dist = Vector3(viewMatrix_xor.get_location().x, viewMatrix_xor.get_location().y, viewMatrix_xor.get_location().z).DistTo(PreditPos);

								AimCorrection22(&PreditPos, Vel, dist, Config::predit_level2);
							}
							Vector2 Vec2 = predit ? viewMatrix.WorldToScreen(PreditPos) : viewMatrix.WorldToScreen(RootPos);
							float CrossDist = CrossHair.Distance(Vec2);

							if (CrossDist < origin && CrossDist <= Config::Fov && !Config::fov360)
							{
								target = predit ? PreditPos : RootPos;
								origin = CrossDist;
								TarGetIndex = i;
								/*Config::health = entities[i].PlayerHealth;
								if (GetAsyncKeyState(Config::aim_key)) {
									Config::Targetenemyi = i;
									Config::targetenemy = convertToHex(Config::targetargb);
									SDK->WPM<uint32_t>(entities[i].OutlineBase + 0x144, Config::targetenemy);
									SDK->WPM<uint32_t>(entities[i].OutlineBase + 0x130, Config::targetenemy);
								}*/
							}
							else if (Config::fov360) {
								CrossDist = Vector3(viewMatrix_xor.get_location().x, viewMatrix_xor.get_location().y, viewMatrix_xor.get_location().z).DistTo(PreditPos);
								if (CrossDist < origin)
								{
									target = predit ? PreditPos : RootPos;
									origin = CrossDist;
									TarGetIndex = i;
								}
							}

						}

					}
				}
				if (TarGetIndex != -1) {
					Config::health = entities[TarGetIndex].PlayerHealth;
					Config::Targetenemyi = TarGetIndex;
					if (Config::autobone2 && entities[TarGetIndex].HeroID != 0x16dd && entities[TarGetIndex].HeroID != 0x16ee) {
						if (entities[TarGetIndex].HeroID == eHero::HERO_TRAININGBOT1 || entities[TarGetIndex].HeroID == eHero::HERO_TRAININGBOT2 || entities[TarGetIndex].HeroID == eHero::HERO_TRAININGBOT3 || entities[TarGetIndex].HeroID == eHero::HERO_TRAININGBOT4 || entities[TarGetIndex].HeroID == eHero::HERO_TRAININGBOT5 || entities[TarGetIndex].HeroID == eHero::HERO_TRAININGBOT6 || entities[TarGetIndex].HeroID == eHero::HERO_TRAININGBOT7) {
							float distbone[5] = { 0 };
							int index[] = { 17,16,3,13,54 };
							Vector3 bonerootpos{};
							Vector2 bonecrosspos{};
							for (int iii = 0; iii < 5; iii++) {
								bonerootpos = entities[TarGetIndex].GetBonePos(index[iii]);
								if (bonerootpos == entities[TarGetIndex].pos) distbone[iii] = 100000;
								else {
									bonecrosspos = viewMatrix.WorldToScreen(bonerootpos);
									distbone[iii] = CrossHair.Distance(bonecrosspos);
								}
							}
							int m;
							m = std::min_element(distbone, distbone + 5) - distbone;
							RootPos = entities[TarGetIndex].GetBonePos(index[m]);
							PreditPos = RootPos;
							target = RootPos;
							if (predit)
							{
								Vel = entities[TarGetIndex].velocity;
								float dist = Vector3(viewMatrix_xor.get_location().x, viewMatrix_xor.get_location().y, viewMatrix_xor.get_location().z).DistTo(PreditPos);
								AimCorrection(&PreditPos, Vel, dist, Config::predit_level);
								target = PreditPos;
							}
						}
						else {
							float distbone[10] = { 0 };
							Vector3 bonerootpos{};
							Vector2 bonecrosspos{};
							for (int iii = 0; iii < 10; iii++) {
								bonerootpos = entities[TarGetIndex].GetBonePos(entities[TarGetIndex].GetSkel()[iii]);
								bonecrosspos = viewMatrix.WorldToScreen(bonerootpos);
								distbone[iii] = CrossHair.Distance(bonecrosspos);
							}
							int m;
							m = std::min_element(distbone, distbone + 10) - distbone;
							RootPos = entities[TarGetIndex].GetBonePos(entities[TarGetIndex].GetSkel()[m]);
							PreditPos = RootPos;
							target = RootPos;
							if (predit)
							{
								Vel = entities[TarGetIndex].velocity;
								float dist = Vector3(viewMatrix_xor.get_location().x, viewMatrix_xor.get_location().y, viewMatrix_xor.get_location().z).DistTo(PreditPos);
								AimCorrection(&PreditPos, Vel, dist, Config::predit_level);
								target = PreditPos;
							}
						}
					}
					if (GetAsyncKeyState(Config::aim_key2) && Config::externaloutline) {
						Config::targetenemy = convertToHex(Config::targetargb2);
						SDK->WPM<uint32_t>(entities[TarGetIndex].OutlineBase + 0x144, Config::targetenemy);
						SDK->WPM<uint32_t>(entities[TarGetIndex].OutlineBase + 0x130, Config::targetenemy);
					}
				}
			}
		}
		return target;
	}
	Vector3 GetVector3forfov(bool predit = false)
	{
		int TarGetIndex = -1;
		Vector3 target{};
		Vector2 CrossHair = Vector2(GetSystemMetrics(SM_CXSCREEN) / 2.0f, GetSystemMetrics(SM_CYSCREEN) / 2.0f);

		float origin = 100000.f;
		float dist = 100000.f;
		if (entities.size() > 0) {
			{
				Vector3 Vel;
				Vector3 PreditPos;
				Vector3 RootPos;
				for (int i = 0; i < entities.size(); i++)
				{
					if (!Config::switch_team) {
						if (entities[i].Alive && entities[i].Team && entities[i].Vis)
						{
							PreditPos = entities[i].head_pos;
							RootPos = entities[i].head_pos;
							Vel = entities[i].velocity;
							Vector2 Vec2 = predit ? viewMatrix.WorldToScreen(PreditPos) : viewMatrix.WorldToScreen(RootPos);
							float CrossDist = CrossHair.Distance(Vec2);
							if (CrossDist < origin)
							{
								target = predit ? PreditPos : RootPos;
								origin = CrossDist;
								TarGetIndex = i;
							}
						}
					}
					else if (Config::switch_team) {
						if (entities[i].Alive && !entities[i].Team && entities[i].Vis && entities[i].address != local_entity.address)
						{
							PreditPos = entities[i].head_pos;
							RootPos = entities[i].head_pos;
							Vel = entities[i].velocity;
							Vector2 Vec2 = predit ? viewMatrix.WorldToScreen(PreditPos) : viewMatrix.WorldToScreen(RootPos);
							float CrossDist = CrossHair.Distance(Vec2);
							if (CrossDist < origin)
							{
								target = predit ? PreditPos : RootPos;
								origin = CrossDist;
								TarGetIndex = i;
							}
						}
					}
				}
				if (TarGetIndex != -1) {
					Config::health = entities[TarGetIndex].PlayerHealth;
					Config::Targetenemyifov = TarGetIndex;
				}
			}
		}
		return target;
	}

}