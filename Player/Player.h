#pragma once

#include "stdafx.h"
#include "resource.h"
#include <iostream>
#include "windows.h"
#include "commctrl.h"
#include <stdio.h>
#include <conio.h>
#include <filesystem>
#include <string>
#include <string.h>
#include <map>
using namespace std;
#include <Mmsystem.h>
//#pragma comment(lib, "winmm.lib") 

// define
#define ID_BUTTON_RANDOM 1
#define ID_BUTTON_PREW 2
#define ID_BUTTON_PLAY 3
#define ID_BUTTON_NEXT 4
#define ID_BUTTON_MUTE 5
#define ID_LISTBOX_SONG 6
#define ID_STATIC_SONG_NAME 7
#define ID_TIMER_SONG 8
#define ID_STATIC_TIMER_SONG 9
void updateTime() {
	OutputDebugString(L"zz\n");
}
// class
class App
{
private:
	HWND hWnd;
	UINT message;
	WPARAM wParam;
	LPARAM lParam;
	string folderPath = "";
	HDC hdc;
	map<string, HWND> hWnds;
	int width, height;
	// song
	vector<WIN32_FIND_DATA> songs;
	int indexSong = -1, time = 0, songConfig = SND_FILENAME | SND_ASYNC;
	// status
	bool playing = false, looping = false;
public:
	App() {

	}
	~App() {

	}
	vector<WIN32_FIND_DATA> getSongs(LPCWSTR path) {
		vector<WIN32_FIND_DATA> files;
		WIN32_FIND_DATA data;
		HANDLE hFind = FindFirstFile(path, &data);
		if (hFind != INVALID_HANDLE_VALUE) {
			do {
				if (_wcsicmp(data.cFileName, L".") == 0) continue;
				if (_wcsicmp(data.cFileName, L"..") == 0) continue;
				files.insert(files.end(), data);
			} while (FindNextFile(hFind, &data));
			FindClose(hFind);
		}
		return files;
	}
	void selectFolder() {
		songs = this->getSongs(L"C:/music/*.*");

		vector<WIN32_FIND_DATA>::iterator song;
		SendMessage(hWnds["listbox_songs"], LB_RESETCONTENT, 0, 0);
		int i = 0;
		for (song = songs.begin(); song != songs.end(); song++) {
			SendMessage(hWnds["listbox_songs"], LB_INSERTSTRING, i++, (LPARAM)song->cFileName); // addd
		}
	}
	void size() {
		width = LOWORD(lParam);
		height = HIWORD(lParam);
	}
	void paint(HDC hdc) {
		HPEN hPen1 = CreatePen(PS_SOLID, -1, NULL);
		HBRUSH hBrush1 = CreateSolidBrush(RGB(9, 18, 35));

		HPEN hPen2 = CreatePen(PS_SOLID, 1, RGB(244, 248, 252));
		HBRUSH hBrush2 = CreateSolidBrush(RGB(244, 248, 252));

		HBRUSH hBrush3 = CreateSolidBrush(RGB(13, 37, 63));

		SelectObject(hdc, hPen1);

		SelectObject(hdc, hBrush1);
		Rectangle(hdc, 0, 450, width, height);

		SelectObject(hdc, hPen2);
		SelectObject(hdc, hBrush2);
		Rectangle(hdc, 400, 28, width, 450);
	}
	void set(HWND &hWnd, UINT &message, WPARAM &wParam, LPARAM &lParam) {
		this->hWnd = hWnd;
		this->message = message;
		this->wParam = wParam;
		this->lParam = lParam;
	}
	void setColor() {
		HDC hdc = GetDC(hWnds["static_song_name"]);
		SetTextColor(hdc, RGB(255, 0, 0)); // red
		SetBkColor(hdc, RGB(255, 255, 0)); // yellow
	}
	void create(HINSTANCE &hInstance) {
		hWnds["static_song_name"] = CreateWindow(L"static", L"Socola", WS_VISIBLE | WS_CHILD, 7, 7, 700, 20, hWnd, (HMENU)ID_STATIC_SONG_NAME, NULL, NULL);
		hWnds["static_timer_song"] = CreateWindow(L"static", L"00:00", WS_VISIBLE | WS_CHILD, 180, 473, 50, 25, hWnd, (HMENU)ID_STATIC_TIMER_SONG, NULL, NULL);
		//hWnds["button_random"] = CreateWindow(L"button", L"rand", WS_VISIBLE | WS_CHILD, 220, 473, 25, 25, hWnd, (HMENU)ID_BUTTON_RANDOM, NULL, NULL);
		hWnds["button_prew"] = CreateWindow(L"button", L"Prew", WS_VISIBLE | WS_CHILD, 250, 473, 50, 25, hWnd, (HMENU)ID_BUTTON_PREW, NULL, NULL);
		hWnds["button_play"] = CreateWindow(L"button", L"Play", WS_VISIBLE | WS_CHILD, 300, 460, 50, 50, hWnd, (HMENU) ID_BUTTON_PLAY, NULL, NULL);
		hWnds["button_next"] = CreateWindow(L"button", L"Next", WS_VISIBLE | WS_CHILD, 350, 473, 50, 25, hWnd, (HMENU)ID_BUTTON_NEXT, NULL, NULL);
		//hWnds["button_mute"] = CreateWindow(L"button", L"mu", WS_VISIBLE | WS_CHILD, 400, 473, 25, 25, hWnd, (HMENU)ID_BUTTON_MUTE, NULL, NULL);
		hWnds["listbox_songs"] = CreateWindow(L"listbox", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | TRANSPARENT, 400, 28, 280, 433, hWnd, (HMENU)ID_LISTBOX_SONG, NULL, NULL);
		this->selectFolder();

		HFONT hFont = CreateFont(NULL, 9, 0, 1000, 400, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, NULL, TEXT(".vnTime"));
		map<string, HWND>::iterator it;
		for (it = hWnds.begin(); it != hWnds.end(); it++) {
			SendMessage(it->second, WM_SETFONT, WPARAM(hFont), TRUE);
		}

		HANDLE hbmp = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP2), IMAGE_BITMAP, NULL, NULL, LR_DEFAULTCOLOR);
		SendMessage(hWnds["button_play"], BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)hbmp);


		//HBRUSH hBrush2 = CreateSolidBrush(RGB(244, 248, 252));
		//SetTextColor(hdc, RGB(244, 248, 252));
		//SendMessage(hWnds["listbox_songs"], WM_set, (WPARAM)hFont, TRUE);
	}
	void updateTime() {
		time++;
		TCHAR stime[1000];
		int m = time / 60;
		int s = time % 60;
		wsprintf(stime, L"%2d:%2d", m, s);
		SetDlgItemText(hWnd, ID_STATIC_TIMER_SONG, stime);
	}
	// song
	void loopSong() {
		if (looping) {
			songConfig &= SND_LOOP;
		}
		else {
			songConfig |= SND_LOOP;
		}
		playSong();
	}
	void selectSong() {
		pauseSong();
		playSong();
	}
	void playSong() {
		if (playing) {
			return pauseSong();
		}
		playing = true;
		time = 0;
		indexSong = SendMessage(hWnds["listbox_songs"], LB_GETCURSEL, 0, 0);
		if (indexSong == -1) {
			return;
		}
		WIN32_FIND_DATA song = songs.at(indexSong);
		TCHAR path[1000];
		wsprintf(path, L"C:/music/%s", song.cFileName);
		PlaySound(path, NULL, songConfig);
		SetDlgItemText(hWnd, ID_STATIC_SONG_NAME, song.cFileName);
		KillTimer(hWnd, NULL);
		SetTimer(hWnd, NULL, 1000, NULL);
	}
	void pauseSong() {
		KillTimer(hWnd, NULL);
		time = 0;
		playing = false;
		PlaySound(NULL, NULL, songConfig);
	}
	void nextSong() {
		indexSong++;
		if (indexSong >= songs.size()) {
			indexSong = 0;
		}
		SendMessage(hWnds["listbox_songs"], LB_SETCURSEL, indexSong, 0);
		this->selectSong();
	}
	void prewSong() {
		indexSong--;
		if (indexSong < 0) {
			indexSong = songs.size() - 1;
		}
		SendMessage(hWnds["listbox_songs"], LB_SETCURSEL, indexSong, 0);
		this->selectSong();
	}
};