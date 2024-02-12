#pragma once
#include <string>
namespace OW {
	namespace Config {

		bool loginornot = 1;
		bool LoggedIn = false;
		int got = 0;

		bool namespoofer = false;
		char fakename[] = u8"猎户座用户#1234";

		uint8_t trytoplus = 0;

		bool enablechangefov = false;
		float CHANGEFOV = 103;


		/* Aimbot */
		bool aiaim = false;
		bool lockontarget = false;
		bool highPriority = false;
		bool hanzoautospeed = false;
		float Flick_smooth = 0.1f;
		float bladespeed = 0.5f;
		float Flick_smooth2 = 0.1f;

		float Tracking_smooth = 0.1f;

		float Tracking_smooth2 = 0.1f;


		bool autoscalefov = false;

		float minFov1 = 200;
		float minFov2 = 200;

		float Fov = 200.f;

		float Fov2 = 200.f;

		float y_pitch = 0.45f;
		float hitbox = 0.130f;

		float hitbox2 = 0.130f;

		float accvalue = 0.75f;

		float accvalue2 = 0.75f;

		float missbox = 0.6f;

		float predit_level = 110.f; // 家傈 160.f

		int deleteraw = 0;
		int makesuredelete = 0;

		bool secondaim = false;

		bool Flick = true;
		bool Tracking = false;

		bool trackcompensate = false;
		float comarea = 0;
		float comspeed = 0;
		bool widowautounscope = false;

		bool Prediction = false;
		bool hanzo_flick = false;
		bool shooted = false;
		bool shooted2 = false;
		bool triggerbot = false;
		bool silent = false;
		bool silenttrace = false;
		bool fakesilent = false;

		bool Flick2 = false;
		bool Tracking2 = false;
		bool Prediction2 = false;
		bool Gravitypredit2 = false;
		float predit_level2 = 110.0f;

		bool Antiaim = false;

		bool autobone2 = false;


		bool autobone = false;
		bool dontshot = false;
		int shotmanydont = false;
		bool targetdelay = false;
		int targetdelaytime = false;
		int shotcount = 0;
		int timebeforedelay = 0;
		int doingdelay = 0;

		int doingentity = 1;

		bool Gravitypredit = false;

		bool GenjiBlade = false;
		int Qstarttime = 0;
		int Qtime = 12000;

		bool Rage = false;
		bool fov360 = false;
		bool AutoShoot = false;
		int Shoottime = 500;

		//hitbox附近延迟开枪
		bool hitboxdelayshoot = false;
		int hiboxdelaytime = 0;

		int lasttime = 0;
		int slasttime = 0;
		float meleedistance = 3;
		float meleehealth = 30;
		bool AutoSkill = false;
		float SkillHealth = 50;
		bool skilled = false;
		bool skillinfo = false;
		float lasthealth = 10;
		bool reloading = false;
		uint64_t lastheroid = -2;

		const char* BoneName = "Head";
		const char* BoneName2 = "Head";
		int Bone = 1;
		int Bone2 = 1;
		float health = 0.f;
		bool AutoMelee = false;
		bool AutoShiftGenji = false;
		bool sskilled = false;
		bool outline = false;
		bool externaloutline = false;
		bool teamoutline = false;

		bool manualsave = false;

		/* Draw */
		bool draw_hp_pack = false;
		bool draw_fov = false;
		bool draw_box = false;
		bool draw_edge = false;
		bool draw_info = false;
		bool draw_skel = false;
		bool drawbox3d = false;
		bool drawline = false;
		bool trackback = false;
		bool healthoutline = false;
		bool rainbowoutline = false;
		bool radar = false;
		bool radarline = false;
		bool healthbar = true;
		bool drawhealth = false;
		bool ult = true;
		bool dist = false;
		bool name = false;
		bool drawbattletag = true;
		bool eyeray = 1;
		bool crosscircle = 1;
		int crss = 0;
		int locx, locy, pon;
		float therad;

		/* Draw Menu */
		bool Menu = true;

		/* Team */
		bool is_team = false;
		bool switch_team = false;
		bool switch_team2 = false;

		//misc
		bool musicplay = false;
		float recoilnum = 0.f;
		bool norecoil = false;
		bool horizonreco = false;

		int aim_key = VK_XBUTTON2;
		int aim_key2 = VK_XBUTTON1;
		int togglekey = 0;

		uint32_t Allycolor = 0xFFFFFF66;
		uint32_t invisenemy = 0xF5E75F66;
		uint32_t targetenemy = 0xAF00ffff;
		uint32_t targetenemy2 = 0xAF00ffff;
		uint32_t visenemy = 0xAF0079FF;
		int Targetenemyi = -1;
		int Targetenemyifov = -1;
		int lastenemy = -1;
		float lastcd1 = 0;

		short pitch = 0;
		short yaw = 0;
		float tracktime = 0;
		bool dotrack = false;
		bool startsilent = 0;
		bool didsilent = 0;


		/* Color */
		ImVec4 EnemyCol = ImVec4(1, 1, 1, 1);
		ImVec4 fovcol = ImVec4(1.0f, 0.9f, 0, 1);
		ImVec4 fovcol2 = ImVec4(0.855, 0.439, 0.839, 0.5);
		ImVec4 enargb = ImVec4(1, 0.3, 0, 1);
		ImVec4 invisenargb = ImVec4(0.4, 0.37, 0.91, 1);
		ImVec4 allyargb = ImVec4(0.4, 1, 1, 1);
		ImVec4 targetargb = ImVec4(1, 1, 0, 1);
		ImVec4 targetargb2 = ImVec4(1, 1, 0.4, 1);

		ImVec4 rainbowargb = ImVec4(0.15, 1, 0, 1);
		int cps1 = 0;
		int cps2 = 1;
		int cps3 = 0;

		//现在英雄
		std::string nowhero = "";
	}
}