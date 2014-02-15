#ifndef NEBULEUSE_AVATAR_H
#define NEBULEUSE_AVATAR_H
#pragma once

#include "cbase.h"
#include <vector>

class NebuleuseAvatar
{
public:
	NebuleuseAvatar();
	unsigned char * GetAvatarRGB();
	void DecompressAvatar();
private:
	unsigned char * s_Avatar;
	size_t s_Height;
	size_t s_Width;
};
extern NebuleuseAvatar *g_NebAvatar;
#endif