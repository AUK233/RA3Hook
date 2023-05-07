#pragma once
#include <Windows.h>
#include <string>

#define FUNCTION_COUNT 192

#ifdef __EDG__
#define DLLEXPORT
#else
#define DLLEXPORT(i, name) __pragma(comment(linker, "/EXPORT:" name "=" __FUNCDNAME__ ",@" #i))
#endif

extern "C" uintptr_t functions[FUNCTION_COUNT] = { 0 };
std::string functionNames[] =
{
	"CloseDriver",
	"DefDriverProc",
	"DriverCallback",
	"DrvGetModuleHandle",
	"GetDriverModuleHandle",
	"NotifyCallbackData",
	"OpenDriver",
	"PlaySound",
	"PlaySoundA",
	"PlaySoundW",
	"SendDriverMessage",
	"WOW32DriverCallback",
	"WOW32ResolveMultiMediaHandle",
	"WOWAppExit",
	"aux32Message",
	"auxGetDevCapsA",
	"auxGetDevCapsW",
	"auxGetNumDevs",
	"auxGetVolume",
	"auxOutMessage",
	"auxSetVolume",
	"joy32Message",
	"joyConfigChanged",
	"joyGetDevCapsA",
	"joyGetDevCapsW",
	"joyGetNumDevs",
	"joyGetPos",
	"joyGetPosEx",
	"joyGetThreshold",
	"joyReleaseCapture",
	"joySetCapture",
	"joySetThreshold",
	"mci32Message",
	"mciDriverNotify",
	"mciDriverYield",
	"mciExecute",
	"mciFreeCommandResource",
	"mciGetCreatorTask",
	"mciGetDeviceIDA",
	"mciGetDeviceIDFromElementIDA",
	"mciGetDeviceIDFromElementIDW",
	"mciGetDeviceIDW",
	"mciGetDriverData",
	"mciGetErrorStringA",
	"mciGetErrorStringW",
	"mciGetYieldProc",
	"mciLoadCommandResource",
	"mciSendCommandA",
	"mciSendCommandW",
	"mciSendStringA",
	"mciSendStringW",
	"mciSetDriverData",
	"mciSetYieldProc",
	"mid32Message",
	"midiConnect",
	"midiDisconnect",
	"midiInAddBuffer",
	"midiInClose",
	"midiInGetDevCapsA",
	"midiInGetDevCapsW",
	"midiInGetErrorTextA",
	"midiInGetErrorTextW",
	"midiInGetID",
	"midiInGetNumDevs",
	"midiInMessage",
	"midiInOpen",
	"midiInPrepareHeader",
	"midiInReset",
	"midiInStart",
	"midiInStop",
	"midiInUnprepareHeader",
	"midiOutCacheDrumPatches",
	"midiOutCachePatches",
	"midiOutClose",
	"midiOutGetDevCapsA",
	"midiOutGetDevCapsW",
	"midiOutGetErrorTextA",
	"midiOutGetErrorTextW",
	"midiOutGetID",
	"midiOutGetNumDevs",
	"midiOutGetVolume",
	"midiOutLongMsg",
	"midiOutMessage",
	"midiOutOpen",
	"midiOutPrepareHeader",
	"midiOutReset",
	"midiOutSetVolume",
	"midiOutShortMsg",
	"midiOutUnprepareHeader",
	"midiStreamClose",
	"midiStreamOpen",
	"midiStreamOut",
	"midiStreamPause",
	"midiStreamPosition",
	"midiStreamProperty",
	"midiStreamRestart",
	"midiStreamStop",
	"mixerClose",
	"mixerGetControlDetailsA",
	"mixerGetControlDetailsW",
	"mixerGetDevCapsA",
	"mixerGetDevCapsW",
	"mixerGetID",
	"mixerGetLineControlsA",
	"mixerGetLineControlsW",
	"mixerGetLineInfoA",
	"mixerGetLineInfoW",
	"mixerGetNumDevs",
	"mixerMessage",
	"mixerOpen",
	"mixerSetControlDetails",
	"mmDrvInstall",
	"mmGetCurrentTask",
	"mmTaskBlock",
	"mmTaskCreate",
	"mmTaskSignal",
	"mmTaskYield",
	"mmioAdvance",
	"mmioAscend",
	"mmioClose",
	"mmioCreateChunk",
	"mmioDescend",
	"mmioFlush",
	"mmioGetInfo",
	"mmioInstallIOProcA",
	"mmioInstallIOProcW",
	"mmioOpenA",
	"mmioOpenW",
	"mmioRead",
	"mmioRenameA",
	"mmioRenameW",
	"mmioSeek",
	"mmioSendMessage",
	"mmioSetBuffer",
	"mmioSetInfo",
	"mmioStringToFOURCCA",
	"mmioStringToFOURCCW",
	"mmioWrite",
	"mmsystemGetVersion",
	"mod32Message",
	"mxd32Message",
	"sndPlaySoundA",
	"sndPlaySoundW",
	"tid32Message",
	"timeBeginPeriod",
	"timeEndPeriod",
	"timeGetDevCaps",
	"timeGetSystemTime",
	"timeGetTime",
	"timeKillEvent",
	"timeSetEvent",
	"waveInAddBuffer",
	"waveInClose",
	"waveInGetDevCapsA",
	"waveInGetDevCapsW",
	"waveInGetErrorTextA",
	"waveInGetErrorTextW",
	"waveInGetID",
	"waveInGetNumDevs",
	"waveInGetPosition",
	"waveInMessage",
	"waveInOpen",
	"waveInPrepareHeader",
	"waveInReset",
	"waveInStart",
	"waveInStop",
	"waveInUnprepareHeader",
	"waveOutBreakLoop",
	"waveOutClose",
	"waveOutGetDevCapsA",
	"waveOutGetDevCapsW",
	"waveOutGetErrorTextA",
	"waveOutGetErrorTextW",
	"waveOutGetID",
	"waveOutGetNumDevs",
	"waveOutGetPitch",
	"waveOutGetPlaybackRate",
	"waveOutGetPosition",
	"waveOutGetVolume",
	"waveOutMessage",
	"waveOutOpen",
	"waveOutPause",
	"waveOutPrepareHeader",
	"waveOutReset",
	"waveOutRestart",
	"waveOutSetPitch",
	"waveOutSetPlaybackRate",
	"waveOutSetVolume",
	"waveOutUnprepareHeader",
	"waveOutWrite",
	"wid32Message",
	"wod32Message"
};

// CloseDriver
__declspec(naked) void func_ce728bca281447bfb5e41026af878318()
{
	DLLEXPORT(1, "CloseDriver");
	__asm { jmp functions[4 * 0] }
}

// DefDriverProc
__declspec(naked) void func_a418f942b3a54d53a21dc384a64faa26()
{
	DLLEXPORT(2, "DefDriverProc");
	__asm { jmp functions[4 * 1] }
}

// DriverCallback
__declspec(naked) void func_b4f1778862c84f2bac2b80dc9886d831()
{
	DLLEXPORT(3, "DriverCallback");
	__asm { jmp functions[4 * 2] }
}

// DrvGetModuleHandle
__declspec(naked) void func_13d651b993c445668502a877a556f344()
{
	DLLEXPORT(4, "DrvGetModuleHandle");
	__asm { jmp functions[4 * 3] }
}

// GetDriverModuleHandle
__declspec(naked) void func_fa9e75431bfb45b3a6d2de7e3a9705df()
{
	DLLEXPORT(5, "GetDriverModuleHandle");
	__asm { jmp functions[4 * 4] }
}

// NotifyCallbackData
__declspec(naked) void func_bc80c7385a3a4cf09f6b29a23290dae0()
{
	DLLEXPORT(6, "NotifyCallbackData");
	__asm { jmp functions[4 * 5] }
}

// OpenDriver
__declspec(naked) void func_7087fde1bc7b4d239e568fde6edacf70()
{
	DLLEXPORT(7, "OpenDriver");
	__asm { jmp functions[4 * 6] }
}

// PlaySound
__declspec(naked) void func_cdad24afcad54506828f54a9def5f976()
{
	DLLEXPORT(8, "PlaySound");
	__asm { jmp functions[4 * 7] }
}

// PlaySoundA
__declspec(naked) void func_3eab8f86482f44d5b389f9e7355cc791()
{
	DLLEXPORT(9, "PlaySoundA");
	__asm { jmp functions[4 * 8] }
}

// PlaySoundW
__declspec(naked) void func_2de5d24b90344d36b5da868584391713()
{
	DLLEXPORT(10, "PlaySoundW");
	__asm { jmp functions[4 * 9] }
}

// SendDriverMessage
__declspec(naked) void func_f1e936d67111454193396caa0fa49e0e()
{
	DLLEXPORT(11, "SendDriverMessage");
	__asm { jmp functions[4 * 10] }
}

// WOW32DriverCallback
__declspec(naked) void func_e22405f426d84524a6113310c84bf043()
{
	DLLEXPORT(12, "WOW32DriverCallback");
	__asm { jmp functions[4 * 11] }
}

// WOW32ResolveMultiMediaHandle
__declspec(naked) void func_05a908ecb0324bdeb272223fa0879faf()
{
	DLLEXPORT(13, "WOW32ResolveMultiMediaHandle");
	__asm { jmp functions[4 * 12] }
}

// WOWAppExit
__declspec(naked) void func_5acbdb712379440aad0761dbf1490301()
{
	DLLEXPORT(14, "WOWAppExit");
	__asm { jmp functions[4 * 13] }
}

// aux32Message
__declspec(naked) void func_d2b957d2ef2e4d9db528bb6d062b1c1a()
{
	DLLEXPORT(15, "aux32Message");
	__asm { jmp functions[4 * 14] }
}

// auxGetDevCapsA
__declspec(naked) void func_6c6fbe4fb7f84eacb0af6036d8953503()
{
	DLLEXPORT(16, "auxGetDevCapsA");
	__asm { jmp functions[4 * 15] }
}

// auxGetDevCapsW
__declspec(naked) void func_53f245bbf5f745d980ec16ac0cf277f9()
{
	DLLEXPORT(17, "auxGetDevCapsW");
	__asm { jmp functions[4 * 16] }
}

// auxGetNumDevs
__declspec(naked) void func_5872837c3b7b495a9e72b1f03643d0f5()
{
	DLLEXPORT(18, "auxGetNumDevs");
	__asm { jmp functions[4 * 17] }
}

// auxGetVolume
__declspec(naked) void func_357f2edff42d4bb58499a00b72985207()
{
	DLLEXPORT(19, "auxGetVolume");
	__asm { jmp functions[4 * 18] }
}

// auxOutMessage
__declspec(naked) void func_c14a682f9efa4e68a61a4cbf7c5122d1()
{
	DLLEXPORT(20, "auxOutMessage");
	__asm { jmp functions[4 * 19] }
}

// auxSetVolume
__declspec(naked) void func_20a8581fd33a45d09862f83b843a47df()
{
	DLLEXPORT(21, "auxSetVolume");
	__asm { jmp functions[4 * 20] }
}

// joy32Message
__declspec(naked) void func_af0cda14afd54a0aa408f6ec064e13ad()
{
	DLLEXPORT(22, "joy32Message");
	__asm { jmp functions[4 * 21] }
}

// joyConfigChanged
__declspec(naked) void func_f69209e076b54f3589483ffe4091232a()
{
	DLLEXPORT(23, "joyConfigChanged");
	__asm { jmp functions[4 * 22] }
}

// joyGetDevCapsA
__declspec(naked) void func_a6859cc524474a4c864f562815e2a8bf()
{
	DLLEXPORT(24, "joyGetDevCapsA");
	__asm { jmp functions[4 * 23] }
}

// joyGetDevCapsW
__declspec(naked) void func_dea70a47bd5646c1b41a5fa4957ba44a()
{
	DLLEXPORT(25, "joyGetDevCapsW");
	__asm { jmp functions[4 * 24] }
}

// joyGetNumDevs
__declspec(naked) void func_e1d4cf521f3a4a54801676a0a77752a6()
{
	DLLEXPORT(26, "joyGetNumDevs");
	__asm { jmp functions[4 * 25] }
}

// joyGetPos
__declspec(naked) void func_2fe0df43509f43f6bbdd8696fa302c2b()
{
	DLLEXPORT(27, "joyGetPos");
	__asm { jmp functions[4 * 26] }
}

// joyGetPosEx
__declspec(naked) void func_ae32c2f0d5bd4030b70b771ee83fc439()
{
	DLLEXPORT(28, "joyGetPosEx");
	__asm { jmp functions[4 * 27] }
}

// joyGetThreshold
__declspec(naked) void func_faf7e09071dc45fc90c2a030c977fa40()
{
	DLLEXPORT(29, "joyGetThreshold");
	__asm { jmp functions[4 * 28] }
}

// joyReleaseCapture
__declspec(naked) void func_ec802ba998b24a8999f30904424bf049()
{
	DLLEXPORT(30, "joyReleaseCapture");
	__asm { jmp functions[4 * 29] }
}

// joySetCapture
__declspec(naked) void func_a33fa029c81a40a490b4c4c774b0dd5f()
{
	DLLEXPORT(31, "joySetCapture");
	__asm { jmp functions[4 * 30] }
}

// joySetThreshold
__declspec(naked) void func_dca8da3b5e08439a81c4251f94ee2231()
{
	DLLEXPORT(32, "joySetThreshold");
	__asm { jmp functions[4 * 31] }
}

// mci32Message
__declspec(naked) void func_3eec995883a9476ba3c95437e226d881()
{
	DLLEXPORT(33, "mci32Message");
	__asm { jmp functions[4 * 32] }
}

// mciDriverNotify
__declspec(naked) void func_8694c5e766b54e6fbc9874a5504de1e7()
{
	DLLEXPORT(34, "mciDriverNotify");
	__asm { jmp functions[4 * 33] }
}

// mciDriverYield
__declspec(naked) void func_e3dc5b4b011540509f75eb171d1ddda5()
{
	DLLEXPORT(35, "mciDriverYield");
	__asm { jmp functions[4 * 34] }
}

// mciExecute
__declspec(naked) void func_915831b1d5794c27b210066be13b62e1()
{
	DLLEXPORT(36, "mciExecute");
	__asm { jmp functions[4 * 35] }
}

// mciFreeCommandResource
__declspec(naked) void func_56c2e4dd647d41a6a346e18910fdbf12()
{
	DLLEXPORT(37, "mciFreeCommandResource");
	__asm { jmp functions[4 * 36] }
}

// mciGetCreatorTask
__declspec(naked) void func_a897f1b577254893b2f3527478eeb8b8()
{
	DLLEXPORT(38, "mciGetCreatorTask");
	__asm { jmp functions[4 * 37] }
}

// mciGetDeviceIDA
__declspec(naked) void func_f674a27c47f04b3cb23cab2487d7dcb9()
{
	DLLEXPORT(39, "mciGetDeviceIDA");
	__asm { jmp functions[4 * 38] }
}

// mciGetDeviceIDFromElementIDA
__declspec(naked) void func_8020118d28bd49acb3a97d852a5ceead()
{
	DLLEXPORT(40, "mciGetDeviceIDFromElementIDA");
	__asm { jmp functions[4 * 39] }
}

// mciGetDeviceIDFromElementIDW
__declspec(naked) void func_27f0430dd8ca4cb28d3e45a94ea91718()
{
	DLLEXPORT(41, "mciGetDeviceIDFromElementIDW");
	__asm { jmp functions[4 * 40] }
}

// mciGetDeviceIDW
__declspec(naked) void func_1b361606b343460d8433cb1781d5543e()
{
	DLLEXPORT(42, "mciGetDeviceIDW");
	__asm { jmp functions[4 * 41] }
}

// mciGetDriverData
__declspec(naked) void func_3188249b63774620af53a66a7550dcea()
{
	DLLEXPORT(43, "mciGetDriverData");
	__asm { jmp functions[4 * 42] }
}

// mciGetErrorStringA
__declspec(naked) void func_323af038feeb4c8089850002265b12a3()
{
	DLLEXPORT(44, "mciGetErrorStringA");
	__asm { jmp functions[4 * 43] }
}

// mciGetErrorStringW
__declspec(naked) void func_710558bacb2242309aa8b4118ccc4359()
{
	DLLEXPORT(45, "mciGetErrorStringW");
	__asm { jmp functions[4 * 44] }
}

// mciGetYieldProc
__declspec(naked) void func_1663e81ea96a4b648e061f8665f65401()
{
	DLLEXPORT(46, "mciGetYieldProc");
	__asm { jmp functions[4 * 45] }
}

// mciLoadCommandResource
__declspec(naked) void func_265964dd4fc04d6db84ae1b972ec9704()
{
	DLLEXPORT(47, "mciLoadCommandResource");
	__asm { jmp functions[4 * 46] }
}

// mciSendCommandA
__declspec(naked) void func_901e8e5f94994e62a0a52fe9da54ae05()
{
	DLLEXPORT(48, "mciSendCommandA");
	__asm { jmp functions[4 * 47] }
}

// mciSendCommandW
__declspec(naked) void func_8fbf46b8d1b04ff29d3fededb539ed18()
{
	DLLEXPORT(49, "mciSendCommandW");
	__asm { jmp functions[4 * 48] }
}

// mciSendStringA
__declspec(naked) void func_cc50303661dd4e7eafbcce8b1f2a52ac()
{
	DLLEXPORT(50, "mciSendStringA");
	__asm { jmp functions[4 * 49] }
}

// mciSendStringW
__declspec(naked) void func_e4c69d307a06462094fe19d0e5c4ad4e()
{
	DLLEXPORT(51, "mciSendStringW");
	__asm { jmp functions[4 * 50] }
}

// mciSetDriverData
__declspec(naked) void func_90a4a9e49e8e419e86b042d541c94be6()
{
	DLLEXPORT(52, "mciSetDriverData");
	__asm { jmp functions[4 * 51] }
}

// mciSetYieldProc
__declspec(naked) void func_e8994c031b4642e7b4822df802f88630()
{
	DLLEXPORT(53, "mciSetYieldProc");
	__asm { jmp functions[4 * 52] }
}

// mid32Message
__declspec(naked) void func_ef63f840efb74a838ac7ef67e989b5e7()
{
	DLLEXPORT(54, "mid32Message");
	__asm { jmp functions[4 * 53] }
}

// midiConnect
__declspec(naked) void func_5e902588d357425780f9504fef810adb()
{
	DLLEXPORT(55, "midiConnect");
	__asm { jmp functions[4 * 54] }
}

// midiDisconnect
__declspec(naked) void func_66e159a03cdb4cb49c43e9973b8e5223()
{
	DLLEXPORT(56, "midiDisconnect");
	__asm { jmp functions[4 * 55] }
}

// midiInAddBuffer
__declspec(naked) void func_5b8ea1637c624bb0871e170e817da73b()
{
	DLLEXPORT(57, "midiInAddBuffer");
	__asm { jmp functions[4 * 56] }
}

// midiInClose
__declspec(naked) void func_112cfddfd6b84b0db344b442e094b571()
{
	DLLEXPORT(58, "midiInClose");
	__asm { jmp functions[4 * 57] }
}

// midiInGetDevCapsA
__declspec(naked) void func_dcd61d86212f454e8a956e1c809d1162()
{
	DLLEXPORT(59, "midiInGetDevCapsA");
	__asm { jmp functions[4 * 58] }
}

// midiInGetDevCapsW
__declspec(naked) void func_e40e8a5bce7040009b3e5563f3a9cf26()
{
	DLLEXPORT(60, "midiInGetDevCapsW");
	__asm { jmp functions[4 * 59] }
}

// midiInGetErrorTextA
__declspec(naked) void func_c011d18d2df94cd4b3097794aeeaac7f()
{
	DLLEXPORT(61, "midiInGetErrorTextA");
	__asm { jmp functions[4 * 60] }
}

// midiInGetErrorTextW
__declspec(naked) void func_e99871544e3d4fad995dfe7ddac70935()
{
	DLLEXPORT(62, "midiInGetErrorTextW");
	__asm { jmp functions[4 * 61] }
}

// midiInGetID
__declspec(naked) void func_d70a935716e548bfbec26b203a94379a()
{
	DLLEXPORT(63, "midiInGetID");
	__asm { jmp functions[4 * 62] }
}

// midiInGetNumDevs
__declspec(naked) void func_4fca63d022e444ae8109f59de5665fce()
{
	DLLEXPORT(64, "midiInGetNumDevs");
	__asm { jmp functions[4 * 63] }
}

// midiInMessage
__declspec(naked) void func_0bc3a04817a34b4d8905681920902a30()
{
	DLLEXPORT(65, "midiInMessage");
	__asm { jmp functions[4 * 64] }
}

// midiInOpen
__declspec(naked) void func_c301e9c05b1a4596882e3167e55865d9()
{
	DLLEXPORT(66, "midiInOpen");
	__asm { jmp functions[4 * 65] }
}

// midiInPrepareHeader
__declspec(naked) void func_0bba40b67a7f44af86adbedfa870463a()
{
	DLLEXPORT(67, "midiInPrepareHeader");
	__asm { jmp functions[4 * 66] }
}

// midiInReset
__declspec(naked) void func_c7e9870355b04c0a977c09f9cdd0f5c9()
{
	DLLEXPORT(68, "midiInReset");
	__asm { jmp functions[4 * 67] }
}

// midiInStart
__declspec(naked) void func_34fb54df079c496b83365b2d98eed3b6()
{
	DLLEXPORT(69, "midiInStart");
	__asm { jmp functions[4 * 68] }
}

// midiInStop
__declspec(naked) void func_c0ae22b2ed4848f69f15803869f19d00()
{
	DLLEXPORT(70, "midiInStop");
	__asm { jmp functions[4 * 69] }
}

// midiInUnprepareHeader
__declspec(naked) void func_8f1a8f59ce0143a693727a00227f2e96()
{
	DLLEXPORT(71, "midiInUnprepareHeader");
	__asm { jmp functions[4 * 70] }
}

// midiOutCacheDrumPatches
__declspec(naked) void func_80a3ac9e0184485f9fd318cf58d7249d()
{
	DLLEXPORT(72, "midiOutCacheDrumPatches");
	__asm { jmp functions[4 * 71] }
}

// midiOutCachePatches
__declspec(naked) void func_5f38702f12d84e0f9f8626b90e16497a()
{
	DLLEXPORT(73, "midiOutCachePatches");
	__asm { jmp functions[4 * 72] }
}

// midiOutClose
__declspec(naked) void func_a0da952f9ca34849a61c9477fb5479d2()
{
	DLLEXPORT(74, "midiOutClose");
	__asm { jmp functions[4 * 73] }
}

// midiOutGetDevCapsA
__declspec(naked) void func_707e0ba6ba0f467784b24b395d80a48f()
{
	DLLEXPORT(75, "midiOutGetDevCapsA");
	__asm { jmp functions[4 * 74] }
}

// midiOutGetDevCapsW
__declspec(naked) void func_78d8ebe1b7964a89841ba1c11fd41504()
{
	DLLEXPORT(76, "midiOutGetDevCapsW");
	__asm { jmp functions[4 * 75] }
}

// midiOutGetErrorTextA
__declspec(naked) void func_e2ccd7ca72294e0789c85a6ca597855c()
{
	DLLEXPORT(77, "midiOutGetErrorTextA");
	__asm { jmp functions[4 * 76] }
}

// midiOutGetErrorTextW
__declspec(naked) void func_1ab50e7dbcb64fe29ea9d5277659d8e3()
{
	DLLEXPORT(78, "midiOutGetErrorTextW");
	__asm { jmp functions[4 * 77] }
}

// midiOutGetID
__declspec(naked) void func_3f6b80cdea1348a78541f9733bba54bd()
{
	DLLEXPORT(79, "midiOutGetID");
	__asm { jmp functions[4 * 78] }
}

// midiOutGetNumDevs
__declspec(naked) void func_74888c5c4358421e8c8f1ea5305a9147()
{
	DLLEXPORT(80, "midiOutGetNumDevs");
	__asm { jmp functions[4 * 79] }
}

// midiOutGetVolume
__declspec(naked) void func_64c99ceb5c02412fbe86a952e9172f20()
{
	DLLEXPORT(81, "midiOutGetVolume");
	__asm { jmp functions[4 * 80] }
}

// midiOutLongMsg
__declspec(naked) void func_85328bc32ee14d1caa6e2cfc1a6fe147()
{
	DLLEXPORT(82, "midiOutLongMsg");
	__asm { jmp functions[4 * 81] }
}

// midiOutMessage
__declspec(naked) void func_308f2db4246f4f0eb1cce9561318ce5a()
{
	DLLEXPORT(83, "midiOutMessage");
	__asm { jmp functions[4 * 82] }
}

// midiOutOpen
__declspec(naked) void func_f7119b31441942c3b07f733a50f73b1f()
{
	DLLEXPORT(84, "midiOutOpen");
	__asm { jmp functions[4 * 83] }
}

// midiOutPrepareHeader
__declspec(naked) void func_7da2225481dd4250b32a4b27e80c7659()
{
	DLLEXPORT(85, "midiOutPrepareHeader");
	__asm { jmp functions[4 * 84] }
}

// midiOutReset
__declspec(naked) void func_34a7bf2cf02c4573b296321d89bb6ce5()
{
	DLLEXPORT(86, "midiOutReset");
	__asm { jmp functions[4 * 85] }
}

// midiOutSetVolume
__declspec(naked) void func_598e278f04cf43cebee7b3b39d179e7c()
{
	DLLEXPORT(87, "midiOutSetVolume");
	__asm { jmp functions[4 * 86] }
}

// midiOutShortMsg
__declspec(naked) void func_d3c78187d7f64a79bcfa7f83cbb98164()
{
	DLLEXPORT(88, "midiOutShortMsg");
	__asm { jmp functions[4 * 87] }
}

// midiOutUnprepareHeader
__declspec(naked) void func_2b9154251a5f4b41ba0ee14f0282b53d()
{
	DLLEXPORT(89, "midiOutUnprepareHeader");
	__asm { jmp functions[4 * 88] }
}

// midiStreamClose
__declspec(naked) void func_765ac10369e84dc4bb5223e3522c2521()
{
	DLLEXPORT(90, "midiStreamClose");
	__asm { jmp functions[4 * 89] }
}

// midiStreamOpen
__declspec(naked) void func_d05ca11e3c994cf681fe38f72b23d0af()
{
	DLLEXPORT(91, "midiStreamOpen");
	__asm { jmp functions[4 * 90] }
}

// midiStreamOut
__declspec(naked) void func_8396a4f06c05474c96749f69d0a8c52a()
{
	DLLEXPORT(92, "midiStreamOut");
	__asm { jmp functions[4 * 91] }
}

// midiStreamPause
__declspec(naked) void func_c65ade4ba72d46599330762383c63f45()
{
	DLLEXPORT(93, "midiStreamPause");
	__asm { jmp functions[4 * 92] }
}

// midiStreamPosition
__declspec(naked) void func_22beeeadfcd54609ba4eb9fc7586990a()
{
	DLLEXPORT(94, "midiStreamPosition");
	__asm { jmp functions[4 * 93] }
}

// midiStreamProperty
__declspec(naked) void func_7f7465f96bba4329834095145671a466()
{
	DLLEXPORT(95, "midiStreamProperty");
	__asm { jmp functions[4 * 94] }
}

// midiStreamRestart
__declspec(naked) void func_bca0ac9835cd4e809ed4b72c6a94191d()
{
	DLLEXPORT(96, "midiStreamRestart");
	__asm { jmp functions[4 * 95] }
}

// midiStreamStop
__declspec(naked) void func_6f4eca72391747e689fbfaf3a6a2f2f6()
{
	DLLEXPORT(97, "midiStreamStop");
	__asm { jmp functions[4 * 96] }
}

// mixerClose
__declspec(naked) void func_83ec3be7d06e4772ac9e2b54cbb27bce()
{
	DLLEXPORT(98, "mixerClose");
	__asm { jmp functions[4 * 97] }
}

// mixerGetControlDetailsA
__declspec(naked) void func_763bd893acce4a06ae9cd253a743b47f()
{
	DLLEXPORT(99, "mixerGetControlDetailsA");
	__asm { jmp functions[4 * 98] }
}

// mixerGetControlDetailsW
__declspec(naked) void func_41917e83a6df4965a34822f48c40308c()
{
	DLLEXPORT(100, "mixerGetControlDetailsW");
	__asm { jmp functions[4 * 99] }
}

// mixerGetDevCapsA
__declspec(naked) void func_e2fe198922114c14a9e63e08f0584014()
{
	DLLEXPORT(101, "mixerGetDevCapsA");
	__asm { jmp functions[4 * 100] }
}

// mixerGetDevCapsW
__declspec(naked) void func_c21c59e5734a4033856cf6d586a1fa97()
{
	DLLEXPORT(102, "mixerGetDevCapsW");
	__asm { jmp functions[4 * 101] }
}

// mixerGetID
__declspec(naked) void func_273814bcc5614b9a9cd9ea0df0921e46()
{
	DLLEXPORT(103, "mixerGetID");
	__asm { jmp functions[4 * 102] }
}

// mixerGetLineControlsA
__declspec(naked) void func_b408bede4eef41efb912b603ac3b51d9()
{
	DLLEXPORT(104, "mixerGetLineControlsA");
	__asm { jmp functions[4 * 103] }
}

// mixerGetLineControlsW
__declspec(naked) void func_683e4f7867ff4ca88f804edad633f9f2()
{
	DLLEXPORT(105, "mixerGetLineControlsW");
	__asm { jmp functions[4 * 104] }
}

// mixerGetLineInfoA
__declspec(naked) void func_585e019e67ba43acb8718fc0f66d5f0e()
{
	DLLEXPORT(106, "mixerGetLineInfoA");
	__asm { jmp functions[4 * 105] }
}

// mixerGetLineInfoW
__declspec(naked) void func_c6a6cc73b53842eda295c6994eaf158a()
{
	DLLEXPORT(107, "mixerGetLineInfoW");
	__asm { jmp functions[4 * 106] }
}

// mixerGetNumDevs
__declspec(naked) void func_80d584873d2a4107a4b8fc1e8f196247()
{
	DLLEXPORT(108, "mixerGetNumDevs");
	__asm { jmp functions[4 * 107] }
}

// mixerMessage
__declspec(naked) void func_cbd47b53162d46658696cf24c63351f0()
{
	DLLEXPORT(109, "mixerMessage");
	__asm { jmp functions[4 * 108] }
}

// mixerOpen
__declspec(naked) void func_39472f0849fb4557ba2a828a48a27b64()
{
	DLLEXPORT(110, "mixerOpen");
	__asm { jmp functions[4 * 109] }
}

// mixerSetControlDetails
__declspec(naked) void func_fd97b33a29b1412d9de75b6f1a302d53()
{
	DLLEXPORT(111, "mixerSetControlDetails");
	__asm { jmp functions[4 * 110] }
}

// mmDrvInstall
__declspec(naked) void func_645c511d7afe41689b808bcf267d31af()
{
	DLLEXPORT(112, "mmDrvInstall");
	__asm { jmp functions[4 * 111] }
}

// mmGetCurrentTask
__declspec(naked) void func_7b33b625ad5743fd9ef72c96b8accfa3()
{
	DLLEXPORT(113, "mmGetCurrentTask");
	__asm { jmp functions[4 * 112] }
}

// mmTaskBlock
__declspec(naked) void func_fc3756f0582049b8a97dda5030a0a80a()
{
	DLLEXPORT(114, "mmTaskBlock");
	__asm { jmp functions[4 * 113] }
}

// mmTaskCreate
__declspec(naked) void func_edb46ac377684c34a47fa6caa6947a03()
{
	DLLEXPORT(115, "mmTaskCreate");
	__asm { jmp functions[4 * 114] }
}

// mmTaskSignal
__declspec(naked) void func_35f6e0261ef84010a2284bcfd16d2a8a()
{
	DLLEXPORT(116, "mmTaskSignal");
	__asm { jmp functions[4 * 115] }
}

// mmTaskYield
__declspec(naked) void func_eab1620c10734bf3ad4dc8c68f349697()
{
	DLLEXPORT(117, "mmTaskYield");
	__asm { jmp functions[4 * 116] }
}

// mmioAdvance
__declspec(naked) void func_108991c6b09d46019009bba5f97f850a()
{
	DLLEXPORT(118, "mmioAdvance");
	__asm { jmp functions[4 * 117] }
}

// mmioAscend
__declspec(naked) void func_68b691fb4382405ca5e2cd4ec1c57e8b()
{
	DLLEXPORT(119, "mmioAscend");
	__asm { jmp functions[4 * 118] }
}

// mmioClose
__declspec(naked) void func_01be91449c0347d88c00bd7d2a1c441d()
{
	DLLEXPORT(120, "mmioClose");
	__asm { jmp functions[4 * 119] }
}

// mmioCreateChunk
__declspec(naked) void func_6b2cde155d9045fd8d9aa1166579ebf4()
{
	DLLEXPORT(121, "mmioCreateChunk");
	__asm { jmp functions[4 * 120] }
}

// mmioDescend
__declspec(naked) void func_99f392c1eba94f9295110377df7e2db1()
{
	DLLEXPORT(122, "mmioDescend");
	__asm { jmp functions[4 * 121] }
}

// mmioFlush
__declspec(naked) void func_65e2834833994a728a9e045c6d348b02()
{
	DLLEXPORT(123, "mmioFlush");
	__asm { jmp functions[4 * 122] }
}

// mmioGetInfo
__declspec(naked) void func_f6625472a7634a398c2634ffa8e84b09()
{
	DLLEXPORT(124, "mmioGetInfo");
	__asm { jmp functions[4 * 123] }
}

// mmioInstallIOProcA
__declspec(naked) void func_fb823ec8219e4c66a969959402e8d50b()
{
	DLLEXPORT(125, "mmioInstallIOProcA");
	__asm { jmp functions[4 * 124] }
}

// mmioInstallIOProcW
__declspec(naked) void func_cc11e6c184f6445a90e69f0ebc69a7b7()
{
	DLLEXPORT(126, "mmioInstallIOProcW");
	__asm { jmp functions[4 * 125] }
}

// mmioOpenA
__declspec(naked) void func_2aaf5ed59dfe43d5abc76b649562144e()
{
	DLLEXPORT(127, "mmioOpenA");
	__asm { jmp functions[4 * 126] }
}

// mmioOpenW
__declspec(naked) void func_39835ea883774aaeb469d46bc31e73d6()
{
	DLLEXPORT(128, "mmioOpenW");
	__asm { jmp functions[4 * 127] }
}

// mmioRead
__declspec(naked) void func_6706557d4f3b4b389e333b26186acae1()
{
	DLLEXPORT(129, "mmioRead");
	__asm { jmp functions[4 * 128] }
}

// mmioRenameA
__declspec(naked) void func_851e868700fe49939a8d65b231b3c5c1()
{
	DLLEXPORT(130, "mmioRenameA");
	__asm { jmp functions[4 * 129] }
}

// mmioRenameW
__declspec(naked) void func_e6dcac9bb43c4498922920d9347b3e93()
{
	DLLEXPORT(131, "mmioRenameW");
	__asm { jmp functions[4 * 130] }
}

// mmioSeek
__declspec(naked) void func_98f8a19619134ad2a6fc452736777494()
{
	DLLEXPORT(132, "mmioSeek");
	__asm { jmp functions[4 * 131] }
}

// mmioSendMessage
__declspec(naked) void func_25645434f5ef46a6abfb744b413ef255()
{
	DLLEXPORT(133, "mmioSendMessage");
	__asm { jmp functions[4 * 132] }
}

// mmioSetBuffer
__declspec(naked) void func_40fdcd1eaa984bdcb7cf1e84549cb1d3()
{
	DLLEXPORT(134, "mmioSetBuffer");
	__asm { jmp functions[4 * 133] }
}

// mmioSetInfo
__declspec(naked) void func_81911442935b4ab5a7409304886a2633()
{
	DLLEXPORT(135, "mmioSetInfo");
	__asm { jmp functions[4 * 134] }
}

// mmioStringToFOURCCA
__declspec(naked) void func_0df802e8be9c40cf8ada44983572847c()
{
	DLLEXPORT(136, "mmioStringToFOURCCA");
	__asm { jmp functions[4 * 135] }
}

// mmioStringToFOURCCW
__declspec(naked) void func_7fdd298b4140456680ec803237dc325b()
{
	DLLEXPORT(137, "mmioStringToFOURCCW");
	__asm { jmp functions[4 * 136] }
}

// mmioWrite
__declspec(naked) void func_06f7341c45cb4c7dae4e9db50986d936()
{
	DLLEXPORT(138, "mmioWrite");
	__asm { jmp functions[4 * 137] }
}

// mmsystemGetVersion
__declspec(naked) void func_6e85587db0c04e2b8b423d99815e039b()
{
	DLLEXPORT(139, "mmsystemGetVersion");
	__asm { jmp functions[4 * 138] }
}

// mod32Message
__declspec(naked) void func_6fc240d9c22c454ca053803cba25a1cc()
{
	DLLEXPORT(140, "mod32Message");
	__asm { jmp functions[4 * 139] }
}

// mxd32Message
__declspec(naked) void func_5983929d0571458eab508f91b4bd344b()
{
	DLLEXPORT(141, "mxd32Message");
	__asm { jmp functions[4 * 140] }
}

// sndPlaySoundA
__declspec(naked) void func_5bf568005fbe46568d60553212cff4a8()
{
	DLLEXPORT(142, "sndPlaySoundA");
	__asm { jmp functions[4 * 141] }
}

// sndPlaySoundW
__declspec(naked) void func_afcf2de1542d434db09f8093b2cbfe38()
{
	DLLEXPORT(143, "sndPlaySoundW");
	__asm { jmp functions[4 * 142] }
}

// tid32Message
__declspec(naked) void func_f08220fab47e4bd1a52cea2add64a197()
{
	DLLEXPORT(144, "tid32Message");
	__asm { jmp functions[4 * 143] }
}

// timeBeginPeriod
__declspec(naked) void func_23b0c34cc60648a3ad8c7f32eac0145d()
{
	DLLEXPORT(145, "timeBeginPeriod");
	__asm { jmp functions[4 * 144] }
}

// timeEndPeriod
__declspec(naked) void func_10c6f4a263f5435cbd05dacb85d2c8e7()
{
	DLLEXPORT(146, "timeEndPeriod");
	__asm { jmp functions[4 * 145] }
}

// timeGetDevCaps
__declspec(naked) void func_c075123c199343f48a5dde04349cf30d()
{
	DLLEXPORT(147, "timeGetDevCaps");
	__asm { jmp functions[4 * 146] }
}

// timeGetSystemTime
__declspec(naked) void func_ab579e911976431d918439c8d0b59d62()
{
	DLLEXPORT(148, "timeGetSystemTime");
	__asm { jmp functions[4 * 147] }
}

// timeGetTime
__declspec(naked) void func_d37fdf509dab4c748ded41ff83ad5af4()
{
	DLLEXPORT(149, "timeGetTime");
	__asm { jmp functions[4 * 148] }
}

// timeKillEvent
__declspec(naked) void func_1ffd7458b7844c5e893bc0a6ecf3bba1()
{
	DLLEXPORT(150, "timeKillEvent");
	__asm { jmp functions[4 * 149] }
}

// timeSetEvent
__declspec(naked) void func_896abd9565a14217b593a3b599e5ee07()
{
	DLLEXPORT(151, "timeSetEvent");
	__asm { jmp functions[4 * 150] }
}

// waveInAddBuffer
__declspec(naked) void func_ad5e341354b5424fa198d1a791fb98b4()
{
	DLLEXPORT(152, "waveInAddBuffer");
	__asm { jmp functions[4 * 151] }
}

// waveInClose
__declspec(naked) void func_87c25aa13f544fa4b5a7548a3ee01634()
{
	DLLEXPORT(153, "waveInClose");
	__asm { jmp functions[4 * 152] }
}

// waveInGetDevCapsA
__declspec(naked) void func_543fa5cd6a3548a090fa96b97c2f52ba()
{
	DLLEXPORT(154, "waveInGetDevCapsA");
	__asm { jmp functions[4 * 153] }
}

// waveInGetDevCapsW
__declspec(naked) void func_6b28e297b8c44fe1a01ce1a735882807()
{
	DLLEXPORT(155, "waveInGetDevCapsW");
	__asm { jmp functions[4 * 154] }
}

// waveInGetErrorTextA
__declspec(naked) void func_ca657edf416c4878b0f425c5f3ff670b()
{
	DLLEXPORT(156, "waveInGetErrorTextA");
	__asm { jmp functions[4 * 155] }
}

// waveInGetErrorTextW
__declspec(naked) void func_a7caad3b17934b08b36645820784a6d3()
{
	DLLEXPORT(157, "waveInGetErrorTextW");
	__asm { jmp functions[4 * 156] }
}

// waveInGetID
__declspec(naked) void func_e0560c13476642468ac7945c6fc407c7()
{
	DLLEXPORT(158, "waveInGetID");
	__asm { jmp functions[4 * 157] }
}

// waveInGetNumDevs
__declspec(naked) void func_dcd6f14e7586494d8af1d37657c174cf()
{
	DLLEXPORT(159, "waveInGetNumDevs");
	__asm { jmp functions[4 * 158] }
}

// waveInGetPosition
__declspec(naked) void func_587d8981296f4b2ba3d95ab609f91f1f()
{
	DLLEXPORT(160, "waveInGetPosition");
	__asm { jmp functions[4 * 159] }
}

// waveInMessage
__declspec(naked) void func_17ae21fc566b438fa8ab030eb48ac87c()
{
	DLLEXPORT(161, "waveInMessage");
	__asm { jmp functions[4 * 160] }
}

// waveInOpen
__declspec(naked) void func_aa22753cca214e8787b056610325d2a1()
{
	DLLEXPORT(162, "waveInOpen");
	__asm { jmp functions[4 * 161] }
}

// waveInPrepareHeader
__declspec(naked) void func_30ddcbceac1f48f389bd9bbabd949035()
{
	DLLEXPORT(163, "waveInPrepareHeader");
	__asm { jmp functions[4 * 162] }
}

// waveInReset
__declspec(naked) void func_8bbbb7f32d5b4e5f8892dc564e6f924a()
{
	DLLEXPORT(164, "waveInReset");
	__asm { jmp functions[4 * 163] }
}

// waveInStart
__declspec(naked) void func_2b6a86361e1f44a59f39c3efdc816b2f()
{
	DLLEXPORT(165, "waveInStart");
	__asm { jmp functions[4 * 164] }
}

// waveInStop
__declspec(naked) void func_a0459394a7cc4e56b7915272ed54d69d()
{
	DLLEXPORT(166, "waveInStop");
	__asm { jmp functions[4 * 165] }
}

// waveInUnprepareHeader
__declspec(naked) void func_67734bcd211a46178cdc10d803373d6e()
{
	DLLEXPORT(167, "waveInUnprepareHeader");
	__asm { jmp functions[4 * 166] }
}

// waveOutBreakLoop
__declspec(naked) void func_5738b7adc4a44cf3bea244a65dac2f55()
{
	DLLEXPORT(168, "waveOutBreakLoop");
	__asm { jmp functions[4 * 167] }
}

// waveOutClose
__declspec(naked) void func_e1cdec8f69f9432fbb0d5883977a265a()
{
	DLLEXPORT(169, "waveOutClose");
	__asm { jmp functions[4 * 168] }
}

// waveOutGetDevCapsA
__declspec(naked) void func_66117932005d46c2a31a4f555bc53225()
{
	DLLEXPORT(170, "waveOutGetDevCapsA");
	__asm { jmp functions[4 * 169] }
}

// waveOutGetDevCapsW
__declspec(naked) void func_c4c090e5e5c341b7a18c41b37363b7ca()
{
	DLLEXPORT(171, "waveOutGetDevCapsW");
	__asm { jmp functions[4 * 170] }
}

// waveOutGetErrorTextA
__declspec(naked) void func_12ca2572eb734735b81de67f24f1896f()
{
	DLLEXPORT(172, "waveOutGetErrorTextA");
	__asm { jmp functions[4 * 171] }
}

// waveOutGetErrorTextW
__declspec(naked) void func_e1db313d726040d190724abc102098d4()
{
	DLLEXPORT(173, "waveOutGetErrorTextW");
	__asm { jmp functions[4 * 172] }
}

// waveOutGetID
__declspec(naked) void func_658a46b4050c4a008ba96d7c1c8633fb()
{
	DLLEXPORT(174, "waveOutGetID");
	__asm { jmp functions[4 * 173] }
}

// waveOutGetNumDevs
__declspec(naked) void func_234fbb87bdde47688c71e20b3ecdfc29()
{
	DLLEXPORT(175, "waveOutGetNumDevs");
	__asm { jmp functions[4 * 174] }
}

// waveOutGetPitch
__declspec(naked) void func_f78695f4033e4d5db37edce223c27b82()
{
	DLLEXPORT(176, "waveOutGetPitch");
	__asm { jmp functions[4 * 175] }
}

// waveOutGetPlaybackRate
__declspec(naked) void func_1eda37a14b484f8792045abb0159eff8()
{
	DLLEXPORT(177, "waveOutGetPlaybackRate");
	__asm { jmp functions[4 * 176] }
}

// waveOutGetPosition
__declspec(naked) void func_2fc893e8dcb44d04981377162ba31861()
{
	DLLEXPORT(178, "waveOutGetPosition");
	__asm { jmp functions[4 * 177] }
}

// waveOutGetVolume
__declspec(naked) void func_9d1ea882e5be4547897d14a376edb177()
{
	DLLEXPORT(179, "waveOutGetVolume");
	__asm { jmp functions[4 * 178] }
}

// waveOutMessage
__declspec(naked) void func_de047d51aac54618be1afffa2586a119()
{
	DLLEXPORT(180, "waveOutMessage");
	__asm { jmp functions[4 * 179] }
}

// waveOutOpen
__declspec(naked) void func_a3a1d0824fc243f292040d4b4483b80a()
{
	DLLEXPORT(181, "waveOutOpen");
	__asm { jmp functions[4 * 180] }
}

// waveOutPause
__declspec(naked) void func_3adfc8469b9141d4be97bf156300d422()
{
	DLLEXPORT(182, "waveOutPause");
	__asm { jmp functions[4 * 181] }
}

// waveOutPrepareHeader
__declspec(naked) void func_3c0d3cd799024d11b9fa8b93c8b92914()
{
	DLLEXPORT(183, "waveOutPrepareHeader");
	__asm { jmp functions[4 * 182] }
}

// waveOutReset
__declspec(naked) void func_007984692d99457ba92c08ff6a87d95a()
{
	DLLEXPORT(184, "waveOutReset");
	__asm { jmp functions[4 * 183] }
}

// waveOutRestart
__declspec(naked) void func_a681f5b905bc41d48a2c188927705281()
{
	DLLEXPORT(185, "waveOutRestart");
	__asm { jmp functions[4 * 184] }
}

// waveOutSetPitch
__declspec(naked) void func_b00770eee8144e3cb12081da2fdc419c()
{
	DLLEXPORT(186, "waveOutSetPitch");
	__asm { jmp functions[4 * 185] }
}

// waveOutSetPlaybackRate
__declspec(naked) void func_10615e5bccd14e98abcb58c77857b80c()
{
	DLLEXPORT(187, "waveOutSetPlaybackRate");
	__asm { jmp functions[4 * 186] }
}

// waveOutSetVolume
__declspec(naked) void func_a656cd9882da499da546c54b8ab75b8f()
{
	DLLEXPORT(188, "waveOutSetVolume");
	__asm { jmp functions[4 * 187] }
}

// waveOutUnprepareHeader
__declspec(naked) void func_78f3a7618d7148829deb8d7895b2b714()
{
	DLLEXPORT(189, "waveOutUnprepareHeader");
	__asm { jmp functions[4 * 188] }
}

// waveOutWrite
__declspec(naked) void func_49d4055906694175bc92bae87c7225a8()
{
	DLLEXPORT(190, "waveOutWrite");
	__asm { jmp functions[4 * 189] }
}

// wid32Message
__declspec(naked) void func_cbb4d73e9df842c4ae3df19bfa5d6a5f()
{
	DLLEXPORT(191, "wid32Message");
	__asm { jmp functions[4 * 190] }
}

// wod32Message
__declspec(naked) void func_de7bd8658fc9404396034fd4cf91404e()
{
	DLLEXPORT(192, "wod32Message");
	__asm { jmp functions[4 * 191] }
}
