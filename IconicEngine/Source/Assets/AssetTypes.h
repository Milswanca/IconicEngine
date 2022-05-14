#pragma once
const static unsigned int INVALID_ASSET_ID = 100000;
const static unsigned int MAX_ASSETS = 99999;

const static unsigned long INVALID_ASSET_HANDLE = (INVALID_ASSET_ID << 0);

// 64 Bits split
//xxx----- unused
//---x---- generation
//----xxxx resource index;
typedef unsigned long ASSET_HANDLE;

// 64 Bits split
//xxx----- unused
//---x---- generation
//----xxxx resource index;
typedef unsigned long PACKAGE_HANDLE;