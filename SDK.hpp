#pragma once
#include "includes.hpp"
#include <atlstr.h>
#include <stdlib.h>
#include<string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <cstdint>
#include"spoofcall.h"

namespace OW {
	class MemorySDK {
	private:
		std::vector<MEMORY_BASIC_INFORMATION64> mbis;
		bool CompressMbis = false;

		inline uintptr_t GetModuleBaseAddress(DWORD procId, const char* modName)
		{
			uintptr_t modBaseAddr = 0;
			HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procId);
			if (hSnap != INVALID_HANDLE_VALUE)
			{
				MODULEENTRY32 modEntry;
				modEntry.dwSize = sizeof(modEntry);
				if (Module32First(hSnap, &modEntry))
				{
					do
					{
						if (!_stricmp(modEntry.szModule, modName))
						{
							modBaseAddr = (uintptr_t)modEntry.modBaseAddr;
							break;
						}
					} while (Module32Next(hSnap, &modEntry));
				}
			}
			CloseHandle(hSnap);
			return modBaseAddr;
		}
	public:
		HANDLE hProcess = 0;
		uint64_t dwGameBase = 0;
		uint64_t GlobalKey1 = 0, GlobalKey2 = 0;
		uint64_t g_player_controller = 0;
		HWND Windowsizehd = NULL;
	public:
		inline bool Initialize()
		{
			DWORD dwPID;
			Windowsizehd = FindWindowA(skCrypt("TankWindowClass"), NULL);
			GetWindowThreadProcessId(FindWindowA(skCrypt("TankWindowClass"), NULL), &dwPID);

			hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, dwPID);
			dwGameBase = GetModuleBaseAddress(dwPID, skCrypt("Overwatch.exe"));

			return dwGameBase;
		}

		uint64_t Spoocall1(uint64_t* a1, uint64_t* a2)
		{
			return spoof_call((PVOID)(this->dwGameBase + 0x1D21D0C), reinterpret_cast<uint64_t(__fastcall*)(uint64_t*, uint64_t*)>(this->dwGameBase + 0xB2DE0), a2, a1); //48 8b 89 ? ? ? ? e9 ? ? ? ? 83 ff + D, 48 89 74 24 ? 57 48 8b f2 48 8b f9
		}
		bool GetGlobalKey() {
			static auto key_sig = (BYTE*)"\xFF\xFF\xFF\xFF\xFF\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x02\x00\x00\x00";
			static auto key_mask = "xxxxxxxx????????????????????x???";
			while (true) {
				uint64_t Key = FindPatternExReg(key_sig, key_mask, 0x100000);
				printf("%llx\n", Key);
				if (Key && Key < 0xf000000000000000 && RPM<uint64_t>(Key - 0xF0) > 0x100000000000000 && RPM<uint64_t>(Key - 0x78) > 0x100000000000000) {
					GlobalKey1 = RPM<uint64_t>(Key - 0xF0);
					GlobalKey2 = RPM<uint64_t>(Key - 0x78);
					printf("GlobalKey1: 0x%llx\n", GlobalKey1);
					printf("GlobalKey2: 0x%llx\n", GlobalKey2);
					return true;
				}
				Sleep(1000);
			}
		}

		template <typename WPMType>
		__forceinline bool WPM(DWORD_PTR Address, WPMType Buffer)
		{
			__try {
				return WriteProcessMemory(hProcess, LPVOID(Address), &Buffer, sizeof(Buffer), nullptr);
			}
			__except (1) { return 0; }
		}
		/*__forceinline bool WPM(DWORD_PTR Address, WPMType Buffer)
		{
			WPMType* typedAddress = reinterpret_cast<WPMType*>(Address);
			*typedAddress = Buffer;
			return 1;
		}*/

		template <typename RPMType>
		__forceinline RPMType RPM(DWORD_PTR Address)
		{
			RPMType Buffer;
			__try {

				ReadProcessMemory(hProcess, LPVOID(Address), &Buffer, sizeof(Buffer), nullptr);
				return Buffer;
			}
			__except (1) { return Buffer; }
		}

		/*__forceinline RPMType RPM(DWORD_PTR Address)
		{
			RPMType Buffer;
			Buffer=*(reinterpret_cast<RPMType*>(Address));
			return Buffer;
		}*/

		__forceinline uintptr_t calc_relative(uintptr_t current, int32_t relative)
		{
			return current + RPM<int32_t>(current) + relative;
		}
		__forceinline void read_buf(const DWORD_PTR address, char* buffer, SIZE_T size) {
			__try {
				ReadProcessMemory(hProcess, reinterpret_cast<LPVOID>(address), (void*)buffer, size, nullptr);
			}
			__except (1) {
				char a[64] = u8"N/A";
				buffer = a;
			}
		}

		__forceinline void write_buf(const DWORD_PTR address, char* buffer, SIZE_T size) {
			__try {
				WriteProcessMemory(hProcess, LPVOID(address), (void*)buffer, size, nullptr);
			}
			__except (1) {

			}
		}

		__forceinline bool UpdateMemoryQuery()
		{
			MEMORY_BASIC_INFORMATION64 mbi = { 0, };
			MEMORY_BASIC_INFORMATION64 old = { 0, };
			DWORD64 current_address = 0x7ffe0000;
			std::vector<MEMORY_BASIC_INFORMATION64> addresses;
			while (true)
			{
				if (!VirtualQueryEx(hProcess, (LPVOID)current_address, (PMEMORY_BASIC_INFORMATION)&mbi, sizeof(MEMORY_BASIC_INFORMATION64)))
					break;
				if ((mbi.State & 0x1000) != 0 && (mbi.Protect & 0x100) == 0)
				{
					if (old.BaseAddress + old.RegionSize == mbi.BaseAddress && CompressMbis)
						old.RegionSize += mbi.RegionSize;
					else
						addresses.push_back(mbi);

					old = mbi;
				}
				current_address = mbi.BaseAddress + mbi.RegionSize;
			}

			mbis = addresses;


			return (mbis.size() > 0);
		}

		__forceinline DWORD64 FindPattern(BYTE* buffer, BYTE* pattern, std::string mask, int bufSize)
		{
			int pattern_len = mask.length();
			for (int i = 0; i < bufSize - pattern_len; i++)
			{
				bool found = true;
				for (int j = 0; j < pattern_len; j++)
				{
					if (mask[j] != '?' && pattern[j] != buffer[(i + j)])
					{
						found = false;
						break;
					}
				}
				if (found)
					return i;
			}
			return -1;
		}

		__forceinline std::vector<DWORD64> FindPatternEx(DWORD64 start, DWORD64 end, BYTE* pattern, std::string mask, MEMORY_BASIC_INFORMATION64 mbi, DWORD64 RgSize)
		{
			DWORD64 current_chunk = start;
			std::vector<DWORD64> found;
			if ((end - current_chunk > RgSize && RgSize != 0) || (end - current_chunk < RgSize && RgSize != 0))
				return found;
			while (current_chunk < end)
			{
				int bufSize = (int)(end - start);
				BYTE* buffer = new BYTE[bufSize];
				if (!ReadProcessMemory(hProcess, (LPVOID)current_chunk, buffer, bufSize, nullptr))
				{
					current_chunk += bufSize;
					delete[] buffer;
					continue;
				}

				DWORD64 internal_address = FindPattern(buffer, pattern, mask, bufSize);
				if (internal_address != -1)
				{
					found.push_back(current_chunk + internal_address);
				}
				current_chunk += bufSize;
				delete[] buffer;

			}
			return found;
		}

		__forceinline std::vector<DWORD64> FindPatterns(BYTE* buffer, BYTE* pattern, std::string mask, int bufSize)
		{
			std::vector<DWORD64> ret;
			int pattern_len = mask.length();
			for (int i = 0; i < bufSize - pattern_len; i++)
			{
				bool found = true;
				for (int j = 0; j < pattern_len; j++)
				{
					if (mask[j] != '?' && pattern[j] != buffer[i + j])
					{
						found = false;
						break;
					}
				}
				if (found)
					ret.push_back(i);
			}
			return ret;
		}

		__forceinline DWORD64 FindPatternExReg(BYTE* pattern, std::string mask, DWORD64 RgSize)
		{
			if (!UpdateMemoryQuery())
				return 0;

			for (int i = 0; i < mbis.size(); i++) {
				MEMORY_BASIC_INFORMATION64 info = mbis[i];

				std::vector<DWORD64> arr = FindPatternEx(info.BaseAddress, info.RegionSize + info.BaseAddress, pattern, mask, info, RgSize);
				if (arr.size() > 0)
					return arr[0];
			}
			return 0;
		}

		__forceinline std::vector<DWORD64> FindPatternExRegs(BYTE* pattern, std::string mask, DWORD64 RgSize)
		{
			std::vector<DWORD64> Result;
			if (!UpdateMemoryQuery())
				return Result;

			for (int i = 0; i < mbis.size(); i++) {
				MEMORY_BASIC_INFORMATION64 info = mbis[i];

				std::vector<DWORD64> arr = FindPatternEx(info.BaseAddress, info.RegionSize + info.BaseAddress, pattern, mask, info, RgSize);
				if (arr.size() > 0)
					Result.push_back(arr[0]);
			}

			return Result;
		}

		__forceinline std::vector<DWORD64> FindPatternsExReg(BYTE* pattern, std::string mask, DWORD64 RgSize)
		{
			std::vector<DWORD64> Result;
			DWORD64 EntityStart = FindPatternExReg(pattern, mask, RgSize);
			if (EntityStart)
			{
				for (int i = 0; i < mbis.size(); i++) {
					if (mbis[i].BaseAddress < EntityStart && EntityStart - mbis[i].BaseAddress < mbis[i].RegionSize) {
						EntityStart = mbis[i].BaseAddress;
					}
				}

				BYTE* buf = new BYTE[RgSize];
				ReadProcessMemory(hProcess, LPVOID(EntityStart), buf, RgSize, nullptr);

				std::vector<DWORD64> Pointers = FindPatterns(buf, pattern, mask, RgSize);
				delete[] buf;

				for (int i = 0; i < Pointers.size(); i++)
					Pointers[i] += EntityStart;

				Result = Pointers;
			}

			return Result;
		}

		__forceinline std::vector<DWORD64> FindPatternsExRegs(BYTE* pattern, std::string mask, DWORD64 RgSize)
		{
			std::vector<DWORD64> Result;
			std::vector<DWORD64> StartPointers = FindPatternExRegs(pattern, mask, RgSize);

			for (int i = 0; i < StartPointers.size(); i++)
			{
				for (int j = 0; j < mbis.size(); j++) {
					if (mbis[j].BaseAddress < StartPointers[i] && StartPointers[i] - mbis[j].BaseAddress < mbis[j].RegionSize) {
						StartPointers[i] = mbis[j].BaseAddress;
					}
				}

				BYTE* buf = new BYTE[RgSize];
				ReadProcessMemory(hProcess, LPVOID(StartPointers[i]), buf, RgSize, nullptr);

				std::vector<DWORD64> Pointers = FindPatterns(buf, pattern, mask, RgSize);
				delete[] buf;

				for (int j = 0; j < Pointers.size(); j++)
					Pointers[j] += StartPointers[i];

				for (int j = 0; j < Pointers.size(); j++)
				{
					Result.push_back(Pointers[j]);
				}
			}
			return Result;
		}
	};
	inline auto SDK = std::make_unique<MemorySDK>();
}
