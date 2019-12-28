#ifndef __FONTS__
#define __FONTS__

#include "stb_truetype.h"
#include <vector>
#include <functional>
#include "ResourceTexture.h"

class UIFont
{
public:
	UIFont();
	UIFont(const char* filepath, const char* name, unsigned char* bitmap, uint butmap_size, int width, int height, int line_height, stbtt_fontinfo info);
	~UIFont();

	void CleanUp();

	const char* GetName();
	unsigned char* GetBitmap();
	uint GetBitmapSize();
	int GetWidth();
	int GetHeight();
	int GetLineHeight();
	stbtt_fontinfo GetInfo();
	const char* GetFilePath();

private:
	const char* name = nullptr;
	unsigned char* bitmap = nullptr;
	uint bitmap_size = 0;
	int width = 0;
	int height = 0;
	int line_height = 0;
	stbtt_fontinfo info;
	const char* filepath = nullptr;
};

class UIFontsSystem
{
public:
	UIFontsSystem();//UIMain* main
	~UIFontsSystem();

	void CleanUp();

	void LoadFont(const char* filepath, const char* set_name);
	void UnloadFont(const char* name);
	UIFont* GetFont(const char* name);
	UITexture LoadText(const char* text, UIFont* font, uint size);
	void UnloadText(uint id);

	std::function<uint(unsigned char * buffer, uint buffer_size, uint width, uint height)> LoadTexture;
	std::function<void(uint)> UnloadTexture;

private:
	//UIMain* ui_main = nullptr;
	std::vector<UIFont*> fonts;
};

struct UITexture
{
	UITexture();
	UITexture(const UITexture& copy);
	UITexture(uint texture_id, vec2 texture_size);

	uint	  id = 0;
	vec2 size;
};

#endif //__FONTS__
