#pragma once
#include "Vector.hpp"

namespace OW {
	namespace Render {
		inline void DrawQuadFilled(ImVec2 p1, ImVec2 p2, ImVec2 p3, ImVec2 p4, ImColor color) {
			ImGui::GetBackgroundDrawList()->AddQuadFilled(p1, p2, p3, p4, color);
		}
		inline void DrawHexagon(const ImVec2& p1, const ImVec2& p2, const ImVec2& p3, const ImVec2& p4, const ImVec2& p5, const ImVec2& p6, ImU32 col, float thickness)
		{
			ImGui::GetBackgroundDrawList()->AddHexagon(p1, p2, p3, p4, p5, p6, col, thickness);
		}
		inline void DrawHexagonFilled(const ImVec2& p1, const ImVec2& p2, const ImVec2& p3, const ImVec2& p4, const ImVec2& p5, const ImVec2& p6, ImU32 col)
		{
			ImGui::GetBackgroundDrawList()->AddHexagonFilled(p1, p2, p3, p4, p5, p6, col);
		}
		inline void DrawSeerLikeHealth(float x, float y, int shield, int max_shield, /*int armorType,*/ int health, int max_health) {
			int bg_offset = 3;
			int bar_width = 158;
			// 4steps...2*3=6
			// 38*4=152 152+6 = 158
			// 5steps...2*4=8
			// 30*5=150 150+8 = 158
			float shield_step = 25.0f;

			int shield_25 = 30;
			int steps = 5;
			int armorType = 1;
			if (max_shield == 50) armorType = 1;
			else if (max_shield == 75) armorType = 2;
			else if (max_shield == 100) armorType = 3;
			else if (max_shield == 125) armorType = 5;
			/*if (strncmp(gamemode, "arena", 5) == 0 || armorType == 4) { //not EVO shield (arena or gold armor)
				steps = 4;
				shield_25 = 38;
			}*/

			ImVec2 bg1(x - bar_width / 2 - bg_offset, y);
			ImVec2 bg2(bg1.x - 10, bg1.y - 16);
			ImVec2 bg3(bg2.x + 5, bg2.y - 7);
			ImVec2 bg4(bg3.x + bar_width + bg_offset, bg3.y);
			ImVec2 bg5(bg4.x + 11, bg4.y + 18);
			ImVec2 bg6(x + bar_width / 2 + bg_offset, y);
			DrawHexagonFilled(bg1, bg2, bg3, bg4, bg5, bg6, ImColor(0, 0, 0, 120));


			ImVec2 h1(bg1.x + 3, bg1.y - 4);
			ImVec2 h2(h1.x - 5, h1.y - 8);
			ImVec2 h3(h2.x + (float)health / max_health * bar_width, h2.y);
			ImVec2 h4(h1.x + (float)health / max_health * bar_width, h1.y);
			ImVec2 h3m(h2.x + bar_width, h2.y);
			ImVec2 h4m(h1.x + bar_width, h1.y);
			DrawQuadFilled(h1, h2, h3m, h4m, ImColor(10, 10, 30, 60));
			DrawQuadFilled(h1, h2, h3, h4, ImColor(255, 255, 255));


			ImColor shieldCracked(97, 97, 97);
			ImColor shieldCrackedDark(67, 67, 67);

			ImColor shieldCol;
			ImColor shieldColDark; //not used, but the real seer q has shadow inside
			if (armorType == 1) { //white
				shieldCol = ImColor(247, 247, 247);
				shieldColDark = ImColor(164, 164, 164);
			}
			else if (armorType == 2) { //blue
				shieldCol = ImColor(39, 178, 255);
				shieldColDark = ImColor(27, 120, 210);
			}
			else if (armorType == 3) { //purple
				shieldCol = ImColor(206, 59, 255);
				shieldColDark = ImColor(136, 36, 220);
			}
			else if (armorType == 4) { //gold
				shieldCol = ImColor(255, 255, 79);
				shieldColDark = ImColor(218, 175, 49);
			}
			else if (armorType == 5) { //red
				shieldCol = ImColor(219, 2, 2);
				shieldColDark = ImColor(219, 2, 2);
			}
			else {
				shieldCol = ImColor(247, 247, 247);
				shieldColDark = ImColor(164, 164, 164);
			}
			int shield_tmp = shield;
			int shield1 = 0;
			int shield2 = 0;
			int shield3 = 0;
			int shield4 = 0;
			int shield5 = 0;
			if (shield_tmp > 25) {
				shield1 = 25;
				shield_tmp -= 25;
				if (shield_tmp > 25) {
					shield2 = 25;
					shield_tmp -= 25;
					if (shield_tmp > 25) {
						shield3 = 25;
						shield_tmp -= 25;
						if (shield_tmp > 25) {
							shield4 = 25;
							shield_tmp -= 25;
							shield5 = shield_tmp;
						}
						else {
							shield4 = shield_tmp;
						}
					}
					else {
						shield3 = shield_tmp;
					}
				}
				else {
					shield2 = shield_tmp;
				}
			}
			else {
				shield1 = shield_tmp;
			}
			ImVec2 s1(h2.x - 1, h2.y - 2);
			ImVec2 s2(s1.x - 3, s1.y - 5);
			ImVec2 s3(s2.x + shield1 / shield_step * shield_25, s2.y);
			ImVec2 s4(s1.x + shield1 / shield_step * shield_25, s1.y);
			ImVec2 s3m(s2.x + shield_25, s2.y);
			ImVec2 s4m(s1.x + shield_25, s1.y);

			ImVec2 ss1(s4m.x + 2, s1.y);
			ImVec2 ss2(s3m.x + 2, s2.y);
			ImVec2 ss3(ss2.x + shield2 / shield_step * shield_25, s2.y);
			ImVec2 ss4(ss1.x + shield2 / shield_step * shield_25, s1.y);
			ImVec2 ss3m(ss2.x + shield_25, s2.y);
			ImVec2 ss4m(ss1.x + shield_25, s1.y);

			ImVec2 sss1(ss4m.x + 2, s1.y);
			ImVec2 sss2(ss3m.x + 2, s2.y);
			ImVec2 sss3(sss2.x + shield3 / shield_step * shield_25, s2.y);
			ImVec2 sss4(sss1.x + shield3 / shield_step * shield_25, s1.y);
			ImVec2 sss3m(sss2.x + shield_25, s2.y);
			ImVec2 sss4m(sss1.x + shield_25, s1.y);

			ImVec2 ssss1(sss4m.x + 2, s1.y);
			ImVec2 ssss2(sss3m.x + 2, s2.y);
			ImVec2 ssss3(ssss2.x + shield4 / shield_step * shield_25, s2.y);
			ImVec2 ssss4(ssss1.x + shield4 / shield_step * shield_25, s1.y);
			ImVec2 ssss3m(ssss2.x + shield_25, s2.y);
			ImVec2 ssss4m(ssss1.x + shield_25, s1.y);

			ImVec2 sssss1(ssss4m.x + 2, s1.y);
			ImVec2 sssss2(ssss3m.x + 2, s2.y);
			ImVec2 sssss3(sssss2.x + shield5 / shield_step * shield_25, s2.y);
			ImVec2 sssss4(sssss1.x + shield5 / shield_step * shield_25, s1.y);
			ImVec2 sssss3m(sssss2.x + shield_25, s2.y);
			ImVec2 sssss4m(sssss1.x + shield_25, s1.y);
			if (max_shield == 50) {
				if (shield <= 25) {
					if (shield < 25) {
						DrawQuadFilled(s1, s2, s3m, s4m, shieldCracked);
						DrawQuadFilled(ss1, ss2, ss3m, ss4m, shieldCracked);
					}
					if (shield != 0)
						DrawQuadFilled(s1, s2, s3, s4, shieldCol);

				}
				else if (shield <= 50) {
					DrawQuadFilled(s1, s2, s3, s4, shieldCol);
					if (shield != 50) {
						DrawQuadFilled(ss1, ss2, ss3m, ss4m, shieldCracked);
					}
					if (shield != 0)
						DrawQuadFilled(ss1, ss2, ss3, ss4, shieldCol);
				}
			}
			else if (max_shield == 75) {
				if (shield <= 25) {
					if (shield < 25) {
						DrawQuadFilled(s1, s2, s3m, s4m, shieldCracked);
						DrawQuadFilled(ss1, ss2, ss3m, ss4m, shieldCracked);
						DrawQuadFilled(sss1, sss2, sss3m, sss4m, shieldCracked);
					}
					if (shield != 0)
						DrawQuadFilled(s1, s2, s3, s4, shieldCol);

				}
				else if (shield <= 50) {
					DrawQuadFilled(s1, s2, s3, s4, shieldCol);
					if (shield < 50) {
						DrawQuadFilled(ss1, ss2, ss3m, ss4m, shieldCracked);
						DrawQuadFilled(sss1, sss2, sss3m, sss4m, shieldCracked);
					}
					if (shield != 0)
						DrawQuadFilled(ss1, ss2, ss3, ss4, shieldCol);
				}
				else if (shield <= 75) {
					DrawQuadFilled(s1, s2, s3, s4, shieldCol);
					DrawQuadFilled(ss1, ss2, ss3, ss4, shieldCol);
					if (shield < 75) {
						DrawQuadFilled(sss1, sss2, sss3m, sss4m, shieldCracked);
					}
					if (shield != 0)
						DrawQuadFilled(sss1, sss2, sss3, sss4, shieldCol);
				}
			}
			else if (max_shield == 100) {
				if (shield <= 25) {
					if (shield < 25) {
						DrawQuadFilled(s1, s2, s3m, s4m, shieldCracked);
						DrawQuadFilled(ss1, ss2, ss3m, ss4m, shieldCracked);
						DrawQuadFilled(sss1, sss2, sss3m, sss4m, shieldCracked);
						DrawQuadFilled(ssss1, ssss2, ssss3m, ssss4m, shieldCracked);
					}
					if (shield != 0)
						DrawQuadFilled(s1, s2, s3, s4, shieldCol);

				}
				else if (shield <= 50) {
					DrawQuadFilled(s1, s2, s3, s4, shieldCol);
					if (shield < 50) {
						DrawQuadFilled(ss1, ss2, ss3m, ss4m, shieldCracked);
						DrawQuadFilled(sss1, sss2, sss3m, sss4m, shieldCracked);
						DrawQuadFilled(ssss1, ssss2, ssss3m, ssss4m, shieldCracked);
					}
					if (shield != 0)
						DrawQuadFilled(ss1, ss2, ss3, ss4, shieldCol);
				}
				else if (shield <= 75) {
					DrawQuadFilled(s1, s2, s3, s4, shieldCol);
					DrawQuadFilled(ss1, ss2, ss3, ss4, shieldCol);
					if (shield < 75) {
						DrawQuadFilled(sss1, sss2, sss3m, sss4m, shieldCracked);
						DrawQuadFilled(ssss1, ssss2, ssss3m, ssss4m, shieldCracked);
					}
					if (shield != 0)
						DrawQuadFilled(sss1, sss2, sss3, sss4, shieldCol);
				}
				else if (shield <= 100) {
					DrawQuadFilled(s1, s2, s3, s4, shieldCol);
					DrawQuadFilled(ss1, ss2, ss3, ss4, shieldCol);
					DrawQuadFilled(sss1, sss2, sss3, sss4, shieldCol);
					if (shield < 100) {
						DrawQuadFilled(ssss1, ssss2, ssss3m, ssss4m, shieldCracked);
					}
					if (shield != 0)
						DrawQuadFilled(ssss1, ssss2, ssss3, ssss4, shieldCol);
				}
			}
			else if (max_shield == 125) {
				if (shield <= 25) {
					if (shield < 25) {
						DrawQuadFilled(s1, s2, s3m, s4m, shieldCracked);
						DrawQuadFilled(ss1, ss2, ss3m, ss4m, shieldCracked);
						DrawQuadFilled(sss1, sss2, sss3m, sss4m, shieldCracked);
						DrawQuadFilled(ssss1, ssss2, ssss3m, ssss4m, shieldCracked);
						DrawQuadFilled(sssss1, sssss2, sssss3m, sssss4m, shieldCracked);
					}
					if (shield != 0)
						DrawQuadFilled(s1, s2, s3, s4, shieldCol);

				}
				else if (shield <= 50) {
					DrawQuadFilled(s1, s2, s3, s4, shieldCol);
					if (shield < 50) {
						DrawQuadFilled(ss1, ss2, ss3m, ss4m, shieldCracked);
						DrawQuadFilled(sss1, sss2, sss3m, sss4m, shieldCracked);
						DrawQuadFilled(ssss1, ssss2, ssss3m, ssss4m, shieldCracked);
						DrawQuadFilled(sssss1, sssss2, sssss3m, sssss4m, shieldCracked);
					}
					if (shield != 0)
						DrawQuadFilled(ss1, ss2, ss3, ss4, shieldCol);
				}
				else if (shield <= 75) {
					DrawQuadFilled(s1, s2, s3, s4, shieldCol);
					DrawQuadFilled(ss1, ss2, ss3, ss4, shieldCol);
					if (shield < 75) {
						DrawQuadFilled(sss1, sss2, sss3m, sss4m, shieldCracked);
						DrawQuadFilled(ssss1, ssss2, ssss3m, ssss4m, shieldCracked);
						DrawQuadFilled(sssss1, sssss2, sssss3m, sssss4m, shieldCracked);
					}
					if (shield != 0)
						DrawQuadFilled(sss1, sss2, sss3, sss4, shieldCol);
				}
				else if (shield <= 100) {
					DrawQuadFilled(s1, s2, s3, s4, shieldCol);
					DrawQuadFilled(ss1, ss2, ss3, ss4, shieldCol);
					DrawQuadFilled(sss1, sss2, sss3, sss4, shieldCol);
					if (shield < 100) {
						DrawQuadFilled(ssss1, ssss2, ssss3m, ssss4m, shieldCracked);
						DrawQuadFilled(sssss1, sssss2, sssss3m, sssss4m, shieldCracked);
					}
					if (shield != 0)
						DrawQuadFilled(ssss1, ssss2, ssss3, ssss4, shieldCol);
				}
				else if (shield <= 125) {
					DrawQuadFilled(s1, s2, s3, s4, shieldCol);
					DrawQuadFilled(ss1, ss2, ss3, ss4, shieldCol);
					DrawQuadFilled(sss1, sss2, sss3, sss4, shieldCol);
					DrawQuadFilled(ssss1, ssss2, ssss3, ssss4, shieldCol);
					if (shield < 125) {
						DrawQuadFilled(sssss1, sssss2, sssss3m, sssss4m, shieldCracked);
					}
					if (shield != 0)
						DrawQuadFilled(sssss1, sssss2, sssss3, sssss4, shieldCol);
				}
			}
		}
		std::string string_To_UTF8(const std::string& str) {
			int nwLen = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);
			wchar_t* pwBuf = new wchar_t[nwLen + 1];
			ZeroMemory(pwBuf, nwLen * 2 + 2);
			::MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.length(), pwBuf, nwLen);
			int nLen = ::WideCharToMultiByte(CP_UTF8, 0, pwBuf, -1, NULL, NULL, NULL, NULL);
			char* pBuf = new char[nLen + 1];
			ZeroMemory(pBuf, nLen + 1);
			::WideCharToMultiByte(CP_UTF8, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);
			std::string retStr(pBuf);
			delete[]pwBuf;
			delete[]pBuf;
			pwBuf = NULL;
			pBuf = NULL;
			return retStr;
		}

		inline void DrawFilledRect(float x, float y, float w, float h, const ImU32& color)
		{
			ImDrawList* Draw = ImGui::GetBackgroundDrawList();

			auto a = (color >> 24) & 0xFF;
			auto r = (color >> 16) & 0xFF;
			auto g = (color >> 8) & 0xFF;
			auto b = (color) & 0xFF;

			Draw->AddRectFilled(ImVec2(x, y), ImVec2(x + w, y + h), ImGui::GetColorU32({ r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f }), NULL, NULL);
		}

		inline void DrawStrokeText(const ImVec2& pos, ImU32 col, const char* str, float size = 15.f)
		{
			ImDrawList* Draw = ImGui::GetBackgroundDrawList();

			Draw->AddText(NULL, size, ImVec2(pos.x, pos.y - 1), ImGui::ColorConvertFloat4ToU32(ImVec4(0, 0, 0, 1)), str);
			Draw->AddText(NULL, size, ImVec2(pos.x, pos.y + 1), ImGui::ColorConvertFloat4ToU32(ImVec4(0, 0, 0, 1)), str);
			Draw->AddText(NULL, size, ImVec2(pos.x - 1, pos.y), ImGui::ColorConvertFloat4ToU32(ImVec4(0, 0, 0, 1)), str);
			Draw->AddText(NULL, size, ImVec2(pos.x + 1, pos.y), ImGui::ColorConvertFloat4ToU32(ImVec4(0, 0, 0, 1)), str);
			Draw->AddText(NULL, size, ImVec2(pos.x, pos.y), col, str);
		}

		inline void DrawInfo(const ImVec2& Pos, ImU32 TagCol, int HeightSize, std::string Info, float distance, float EnHealth, float EnHealthMax) {
			ImDrawList* Draw = ImGui::GetBackgroundDrawList();
			ImVec2 InfoSize = ImGui::CalcTextSize(Info.c_str());
			if (distance < 200)  // 병신같은게 1 / distance 안해놓고 걍 연산때려서 그런거아님 
			{
				// 
				if (EnHealth > 0.f)
				{
					Draw->AddRectFilled(ImVec2(Pos.x - (InfoSize.x / 2), Pos.y + (HeightSize / 2)), ImVec2(Pos.x + (InfoSize.x / 2) + 35, Pos.y - (HeightSize / 2)), ImGui::GetColorU32(ImVec4(0.2, 0.2, 0.6, 0.3)));
					Draw->AddRectFilled(ImVec2(Pos.x - (InfoSize.x / 2), Pos.y + (HeightSize / 2)), ImVec2(Pos.x - (InfoSize.x / 2) + 5, Pos.y - (HeightSize / 2)), TagCol);
					Draw->AddRectFilled(ImVec2(Pos.x - (InfoSize.x / 2) + 7, Pos.y + (HeightSize / 2) - 6), ImVec2(Pos.x - (InfoSize.x / 2) + 7 + ((abs((Pos.x - (InfoSize.x / 2) + 7) - (Pos.x + (InfoSize.x / 2))) / EnHealthMax) * EnHealth), Pos.y + (HeightSize / 2) - 2), ImGui::GetColorU32(ImVec4(0, 1, 0, 1)));
					DrawStrokeText(ImVec2(Pos.x - ((InfoSize.x / 2)) + 10, Pos.y - (HeightSize / 2)), ImGui::GetColorU32(ImVec4(1, 0.6, 0.6, 1)), Info.c_str());
					// 여기부분
					//std::cout << Info;
				}
				else
				{
					Draw->AddRectFilled(ImVec2(Pos.x - (InfoSize.x / 2), Pos.y + (HeightSize / 2)), ImVec2(Pos.x + (InfoSize.x / 2) + 35, Pos.y - (HeightSize / 2)), ImGui::GetColorU32(ImVec4(0.2, 0.2, 0.6, 0.3)));
					Draw->AddRectFilled(ImVec2(Pos.x - (InfoSize.x / 2), Pos.y + (HeightSize / 2)), ImVec2(Pos.x - (InfoSize.x / 2) + 5, Pos.y - (HeightSize / 2)), ImGui::GetColorU32(ImVec4(0.4, 0.4, 0.4, 1.0f)));
					Draw->AddRectFilled(ImVec2(Pos.x - (InfoSize.x / 2) + 7, Pos.y + (HeightSize / 2) - 6), ImVec2(Pos.x - (InfoSize.x / 2) + 7 + ((abs((Pos.x - (InfoSize.x / 2) + 7) - (Pos.x + (InfoSize.x / 2))) / EnHealthMax) * EnHealth), Pos.y + (HeightSize / 2) - 2), ImGui::GetColorU32(ImVec4(0, 1, 0, 1)));
					DrawStrokeText(ImVec2(Pos.x - ((InfoSize.x / 2)) + 10, Pos.y - (HeightSize / 2)), ImGui::GetColorU32(ImVec4(1, 0.6, 0.6, 1)), Info.c_str());
				}
			}
			else  // distance >= 200m
			{
				DrawStrokeText(ImVec2(Pos.x - ((InfoSize.x / 2)) + 10, Pos.y - (HeightSize / 2)), ImGui::GetColorU32(ImVec4(1, 0.6, 0.6, 1)), Info.c_str());
			}
		}

		inline void RenderLine(const Vector2& from, const Vector2& to, uint32_t color, float thickness = 1.0f)
		{
			ImDrawList* Draw = ImGui::GetBackgroundDrawList();

			auto a = (color >> 24) & 0xff;
			auto r = (color >> 16) & 0xff;
			auto g = (color >> 8) & 0xff;
			auto b = (color) & 0xff;

			Draw->AddLine(ImVec2(from.X, from.Y), ImVec2(to.X, to.Y), ImGui::GetColorU32({ r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f }), thickness);
		}
		void Help(const char* desc) {
			ImGui::TextDisabled("(?)");
			if (ImGui::IsItemHovered()) {
				ImGui::BeginTooltip();
				ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
				ImGui::TextUnformatted(desc);
				ImGui::PopTextWrapPos();
				ImGui::EndTooltip();
			}
		}
		inline void DrawCorneredBox(int X, int Y, int W, int H, const ImU32& color, int thickness = 0.5f)
		{
			ImDrawList* Draw = ImGui::GetBackgroundDrawList();
			float lineW = (W / 3);
			float lineH = (H / 3);

			////black outlines
			Draw->AddLine(ImVec2(X, Y), ImVec2(X, Y + lineH), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), 3);
			Draw->AddLine(ImVec2(X, Y), ImVec2(X + lineW, Y), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), 3);
			Draw->AddLine(ImVec2(X + W - lineW, Y), ImVec2(X + W, Y), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), 3);
			Draw->AddLine(ImVec2(X + W, Y), ImVec2(X + W, Y + lineH), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), 3);
			Draw->AddLine(ImVec2(X, Y + H - lineH), ImVec2(X, Y + H), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), 3);
			Draw->AddLine(ImVec2(X, Y + H), ImVec2(X + lineW, Y + H), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), 3);
			Draw->AddLine(ImVec2(X + W - lineW, Y + H), ImVec2(X + W, Y + H), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), 3);
			Draw->AddLine(ImVec2(X + W, Y + H - lineH), ImVec2(X + W, Y + H), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), 3);

			//corners
			Draw->AddLine(ImVec2(X, Y), ImVec2(X, Y + lineH), color, thickness);
			Draw->AddLine(ImVec2(X, Y), ImVec2(X + lineW, Y), color, thickness);
			Draw->AddLine(ImVec2(X + W - lineW, Y), ImVec2(X + W, Y), color, thickness);
			Draw->AddLine(ImVec2(X + W, Y), ImVec2(X + W, Y + lineH), color, thickness);
			Draw->AddLine(ImVec2(X, Y + H - lineH), ImVec2(X, Y + H), color, thickness);
			Draw->AddLine(ImVec2(X, Y + H), ImVec2(X + lineW, Y + H), color, thickness);
			Draw->AddLine(ImVec2(X + W - lineW, Y + H), ImVec2(X + W, Y + H), color, thickness);
			Draw->AddLine(ImVec2(X + W, Y + H - lineH), ImVec2(X + W, Y + H), color, thickness);
		}
		inline void DrawEdges(const ImVec2& top, const ImVec2& bot, const ImVec2& base, ImU32 col)
		{
			ImDrawList* Draw = ImGui::GetBackgroundDrawList();
			float scale = (bot.y - top.y) / 3.0f;
			float length = scale / 1;
			Draw->AddLine(ImVec2(base.x - scale, top.y), ImVec2(base.x - scale + length, top.y), col, 2.0f); //  --- Top left
			Draw->AddLine(ImVec2(base.x - scale, top.y), ImVec2(base.x - scale, top.y + length), col, 2.0f); // | Top left
			Draw->AddLine(ImVec2(base.x + scale / 3, top.y), ImVec2(base.x + scale / 3 + length, top.y), col, 2.0f); // --- Top right
			Draw->AddLine(ImVec2(base.x + scale / 3 + length, top.y), ImVec2(base.x + scale / 3 + length, top.y + length), col, 2.0f); // | Top right
			Draw->AddLine(ImVec2(base.x - scale, bot.y), ImVec2(base.x - scale + length, bot.y), col, 2.0f); // --- Bottom left
			Draw->AddLine(ImVec2(base.x - scale, bot.y), ImVec2(base.x - scale, bot.y - length), col, 2.0f); // | Bottom left
			Draw->AddLine(ImVec2(base.x + scale / 3 + length, bot.y), ImVec2(base.x + scale / 3, bot.y), col, 2.0f); // --- Bottom right
			Draw->AddLine(ImVec2(base.x + scale / 3 + length, bot.y), ImVec2(base.x + scale / 3 + length, bot.y - length), col, 2.0f); // | Bottom right
		}
		inline void DrawSKILL(const ImVec2& Pos, std::string skill) {
			ImDrawList* Draw = ImGui::GetBackgroundDrawList();
			ImVec2 InfoSize = ImGui::CalcTextSize(skill.c_str());
			DrawStrokeText(ImVec2(Pos.x, Pos.y), ImGui::GetColorU32(ImVec4(1, 1, 1, 1)), skill.c_str(),19);
			//std::cout << skillstring << std::endl;
		}


		void DrawLine(Vector2 From, Vector2 To, Color color, int thickness) {
			int x1 = From.X;
			int y1 = From.Y;
			int x2 = To.X;
			int y2 = To.Y;

			ImGui::GetForegroundDrawList()->AddLine(ImVec2(x1, y1), ImVec2(x2, y2), ImGui::ColorConvertFloat4ToU32(ImVec4(color.R / 255.0, color.G / 255.0, color.B / 255.0, color.A / 255.0)), thickness);
		}
		void DrawBox(Rect rect, Color color, float thickness) {
			Vector2 v1 = Vector2(rect.x, rect.y);
			Vector2 v2 = Vector2(rect.x + rect.width, rect.y);
			Vector2 v3 = Vector2(rect.x + rect.width, rect.y + rect.height);
			Vector2 v4 = Vector2(rect.x, rect.y + rect.height);

			DrawLine(v1, v2, color, thickness);
			DrawLine(v2, v3, color, thickness);
			DrawLine(v3, v4, color, thickness);
			DrawLine(v4, v1, color, thickness);

		}
		void DrawHealthBar(Vector2 screenPos, float height, float currentHealth, float maxHealth) {
			screenPos.X += 8;
			DrawBox(Rect(screenPos.X, screenPos.Y, 5.0f, height + 2), Color(0, 0, 0), 3);
			screenPos.X += 1;
			screenPos.Y += 1;
			float barHeight = (currentHealth * height) / maxHealth;

			Color HealthColor = Color(10, 255, 10);
			if (currentHealth <= (maxHealth * 0.8)) {
				HealthColor = { 255,255,10 };
			}
			if (currentHealth <= (maxHealth * 0.6)) {
				HealthColor = { 255,150,10 };
			}
			if (currentHealth < (maxHealth * 0.4)) {
				HealthColor = { 255,50,10 };
			}
			if (currentHealth < (maxHealth * 0.2)) {
				HealthColor = { 255,10,10 };
			}
			if (currentHealth < (maxHealth * 0)) {
				HealthColor = { 0,0,0 };
			}
			DrawBox(Rect(screenPos.X, screenPos.Y, 3.0f, barHeight), HealthColor, 3);
		}
		void DrawString(Vector2 Position, Color color, const char* str) {
			int x = Position.X;
			int y = Position.Y;
			ImFont a;
			std::string utf_8_1 = std::string(str);
			std::string utf_8_2 = string_To_UTF8(utf_8_1);
			ImGui::GetForegroundDrawList()->AddText(ImVec2(x, y), ImGui::ColorConvertFloat4ToU32(ImVec4(color.R / 255.0, color.G / 255.0, color.B / 255.0, color.A / 255.0)), utf_8_2.c_str());
		}

	}
}