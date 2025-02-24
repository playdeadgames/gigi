///////////////////////////////////////////////////////////////////////////////
//         Gigi Rapid Graphics Prototyping and Code Generation Suite         //
//        Copyright (c) 2024 Electronic Arts Inc. All rights reserved.       //
///////////////////////////////////////////////////////////////////////////////

//========================================================
// Bidirectional Messages
//========================================================

STRUCT_BEGIN(PreviewMsg_Ping, "The client and server send this periodically to know the other is still alive. It fails to send if the connection closes.")
STRUCT_END()

//========================================================
// Client To Server Messages
//========================================================

STRUCT_BEGIN(PreviewMsgCS_Version, "The client tells the server what Gigi version it is using")
	STRUCT_FIELD(std::string, version, "", "Gigi Version", 0)
STRUCT_END()

ENUM_BEGIN(PreviewMsgCS_Log_Level, "")
    ENUM_ITEM(Info, "")
    ENUM_ITEM(Warn, "")
    ENUM_ITEM(Error, "")
ENUM_END()

STRUCT_BEGIN(PreviewMsgCS_Log, "The client tells the server what Gigi version it is using")
	STRUCT_FIELD(PreviewMsgCS_Log_Level, level, PreviewMsgCS_Log_Level::Info, "The type of message", 0)
	STRUCT_FIELD(std::string, msg, "", "The log message", 0)
STRUCT_END()

#include "PreviewMsgClientToServerVariant.h"

//========================================================
// Server To Client Messages
//========================================================

STRUCT_BEGIN(PreviewMsgSC_VersionResponse, "The server tells the client whether or not the version is ok.")
	STRUCT_FIELD(bool, versionOK, false, "Whether the gigi version is ok or not.", 0)
STRUCT_END()

STRUCT_BEGIN(PreviewMsgSC_LoadGGFile, "The client tells the server to load a gg file")
	STRUCT_FIELD(std::string, fileName, "", "The path and name of the .gg file to load", 0)
	STRUCT_FIELD(bool, preserveState, true, "If true, camera state etc should be preserved", 0)
STRUCT_END()

#include "PreviewMsgServerToClientVariant.h"

//========================================================
// .gguser File Schema
//========================================================

ENUM_BEGIN(GGUserFile_TLASBuildFlags, "D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_PREFER_FAST_TRACE etc")
	ENUM_ITEM(None, "")
	ENUM_ITEM(AllowUpdate, "")
	ENUM_ITEM(AllowCompaction, "")
	ENUM_ITEM(PreferFastTrace, "")
	ENUM_ITEM(PreferFastBuild, "")
	ENUM_ITEM(MinimizeMemory, "")
ENUM_END()

STRUCT_BEGIN(GGUserFile_ImportedTexture, "The details of an imported texture")
	STRUCT_FIELD(std::string, fileName, "", "The image file loaded", 0)
	STRUCT_FIELD(bool, fileIsSRGB, true, "Whether the file is an sRGB file or not", 0)
	STRUCT_FIELD(bool, makeMips, false, "Whether to make mips or no", 0)
	STRUCT_STATIC_ARRAY(int, size, 3, {0 COMMA 0 COMMA 1}, "The size of the image to create", SCHEMA_FLAG_UI_ARRAY_HIDE_INDEX)
	STRUCT_STATIC_ARRAY(float, color, 4, { 1 COMMA 1 COMMA 1 COMMA 1}, "The color of the image to create, or the tint of the loaded file", SCHEMA_FLAG_UI_ARRAY_HIDE_INDEX)
	STRUCT_FIELD(TextureFormat, format, TextureFormat::RGBA8_Unorm_sRGB, "The format of the texture to create", 0)

	STRUCT_STATIC_ARRAY(int, binaryDims, 3, {0 COMMA 0 COMMA 1}, "The size of the image in the binary file", SCHEMA_FLAG_UI_ARRAY_HIDE_INDEX)
	STRUCT_FIELD(TextureFormat, binaryFormat, TextureFormat::RGBA8_Unorm_sRGB, "The format of the binary file", 0)
STRUCT_END()

STRUCT_BEGIN(GGUserFile_ImportedBuffer, "The details of an imported buffer")
	STRUCT_FIELD(std::string, fileName, "", "The file loaded", 0)
	STRUCT_FIELD(bool, CSVHeaderRow, true, "If reading a CSV, and this is true, it will skip everything up to the first newline, to ignore a header row.", 0)
	STRUCT_FIELD(int, structIndex, -1, "the index of the struct if a structured buffer", 0)
	STRUCT_FIELD(DataFieldType, type, DataFieldType::Count, "The data field type, if not a structured buffer", 0)
	STRUCT_FIELD(int, count, 1, "how many items are stored", 0)
	STRUCT_FIELD(GGUserFile_TLASBuildFlags, RT_BuildFlags, GGUserFile_TLASBuildFlags::PreferFastTrace, "D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_PREFER_FAST_TRACE etc", 0)

	STRUCT_FIELD(bool, BLASOpaque, false, "DXR BLAS Option", 0)
	STRUCT_FIELD(bool, BLASNoDuplicateAnyhitInvocations, false, "DXR BLAS Option", 0)
	STRUCT_FIELD(bool, IsAABBs, false, "Set to true if ray tracing AABBs with intersection shaders. Format is Min XYZ, Max XYZ.", 0)
	STRUCT_FIELD(bool, IsMaterialBuffer, false, "Set to true if buffer should represent materials", 0)
STRUCT_END()

STRUCT_BEGIN(GGUserFile_ImportedResource, "The details of an imported resource")
	STRUCT_FIELD(std::string, nodeName, "", "The name of the node this imported texture data is for", 0)
	STRUCT_FIELD(bool, resetEveryFrame, true, "Whether this texture should be reset every frame, or if the technique is allowed to change it over multiple frames", 0)

	STRUCT_FIELD(bool, isATexture, true, "Is it a texture or a buffer", 0)
	STRUCT_FIELD(GGUserFile_ImportedTexture, texture, {}, "Texture data", 0)
	STRUCT_FIELD(GGUserFile_ImportedBuffer, buffer, {}, "Buffer data", 0)
STRUCT_END()

ENUM_BEGIN(GGUserFile_CameraJitterType, "The sequence of the jittered projection matrix")
	ENUM_ITEM(None, "No Jitter")
	ENUM_ITEM(UniformWhite, "Uniform white noise")
	ENUM_ITEM(Halton23, "Halton(2,3)")
ENUM_END()

STRUCT_BEGIN(GGUserFile_Camera, "")
	STRUCT_FIELD(bool, perspective, true, "Perspective if true, orthographic if false.", 0)
	STRUCT_FIELD(bool, leftHanded, true, "Left handed if true, right handed if false.", 0)
	STRUCT_FIELD(bool, reverseZ, true, "If true, reverses the depth values for more precision. https://developer.nvidia.com/content/depth-precision-visualized", 0)
	STRUCT_FIELD(float, nearPlane, 0.1f, "The distance to the near plane.", 0)
	STRUCT_FIELD(float, farPlane, 1000.0f, "The distance to the far plane. Set to zero for infinite Z.", 0)
	STRUCT_FIELD(float, FOV, 45.0f, "Vertical field of view, in degrees", 0)
	STRUCT_FIELD(float, flySpeed, 0.1f, "How fast the WASD keys move in the world", 0)
	STRUCT_FIELD(float, mouseSensitivity, 0.01f, "How fast the mouse rotates the camera", 0)

	STRUCT_FIELD(GGUserFile_CameraJitterType, jitterType, GGUserFile_CameraJitterType::Halton23, "The sequence of the jittered projection matrix", 0)
	STRUCT_FIELD(int, jitterLength, 16, "The length of the sequence used to jitter the jittered projection matrix. 0 means infinite.", 0)

	STRUCT_STATIC_ARRAY(float, startingCameraPos, 3, { 0.0f COMMA 0.0f COMMA -10.0f }, "", SCHEMA_FLAG_UI_ARRAY_HIDE_INDEX)
	STRUCT_STATIC_ARRAY(float, startingCameraAltitudeAzimuth, 2, { 0.0f COMMA 0.0f }, "", SCHEMA_FLAG_UI_ARRAY_HIDE_INDEX)

	STRUCT_STATIC_ARRAY(float, cameraPos, 3, {0.0f COMMA 0.0f COMMA -10.0f}, "", SCHEMA_FLAG_NO_SERIALIZE)
	STRUCT_STATIC_ARRAY(float, cameraAltitudeAzimuth, 2, {0.0f COMMA 0.0f}, "", SCHEMA_FLAG_NO_SERIALIZE)
	STRUCT_FIELD(bool, cameraChanged, false, "", SCHEMA_FLAG_NO_SERIALIZE)
STRUCT_END()

STRUCT_BEGIN(GGUserFile_SystemVars, "")
	STRUCT_FIELD(std::string, iResolution_varName, "iResolution", "", 0)
	STRUCT_FIELD(std::string, iResolution_textureName, "", "", 0)
	STRUCT_FIELD(std::string, iTime_varName, "iTime", "", 0)
	STRUCT_FIELD(std::string, iTimeDelta_varName, "iTimeDelta", "", 0)
	STRUCT_FIELD(std::string, iFrameRate_varName, "iFrameRate", "", 0)
	STRUCT_FIELD(std::string, iFrame_varName, "iFrame", "", 0)
	STRUCT_FIELD(std::string, iMouse_varName, "iMouse", "", 0)
	STRUCT_FIELD(std::string, MouseState_varName, "MouseState", "", 0)
	STRUCT_FIELD(std::string, MouseStateLastFrame_varName, "MouseStateLastFrame", "", 0)
	STRUCT_FIELD(std::string, WindowSize_varName, "WindowSize", "", 0)

	STRUCT_FIELD(GGUserFile_Camera, camera, {}, "The camera settings. Can pluralize if one isn't enough.", 0)
	STRUCT_FIELD(std::string, ProjMtx_textureName, "", "The projection matrix needs a resolution to calculate for. Choose a texture node and that will be used as a resolution.", 0)
	STRUCT_FIELD(std::string, ViewMtx_varName, "ViewMtx", "", 0)
	STRUCT_FIELD(std::string, InvViewMtx_varName, "InvViewMtx", "", 0)
	STRUCT_FIELD(std::string, ProjMtx_varName, "ProjMtx", "", 0)
	STRUCT_FIELD(std::string, InvProjMtx_varName, "InvProjMtx", "", 0)
	STRUCT_FIELD(std::string, ViewProjMtx_varName, "ViewProjMtx", "", 0)
	STRUCT_FIELD(std::string, InvViewProjMtx_varName, "InvViewProjMtx", "", 0)
	STRUCT_FIELD(std::string, JitteredProjMtx_varName, "JitteredProjMtx", "ProjMtx with jitter.", 0)
	STRUCT_FIELD(std::string, InvJitteredProjMtx_varName, "InvJitteredProjMtx", "Inverted ProjMtx with jitter.", 0)
	STRUCT_FIELD(std::string, JitteredViewProjMtx_varName, "JitteredViewProjMtx", "ViewProjMtx with jitter.", 0)
	STRUCT_FIELD(std::string, InvJitteredViewProjMtx_varName, "InvJitteredViewProjMtx", "Inverted ViewProjMtx with jitter.", 0)
	STRUCT_FIELD(std::string, CameraPos_varName, "CameraPos", "", 0)
	STRUCT_FIELD(std::string, CameraAltitudeAzimuth_varName, "CameraAltitudeAzimuth", "", 0)
	STRUCT_FIELD(std::string, CameraChanged_varName, "CameraChanged", "", 0)
	STRUCT_FIELD(std::string, CameraJitter_varName, "CameraJitter", "", 0)
	STRUCT_FIELD(std::string, CameraFOV_varName, "CameraFOV", "", 0)
	STRUCT_FIELD(std::string, CameraNearPlane_varName, "CameraNearPlane", "", 0)
	STRUCT_FIELD(std::string, CameraFarPlane_varName, "CameraFarPlane", "", 0)
	STRUCT_FIELD(std::string, ShadingRateImageTileSize_varName, "ShadingRateImageTileSize", "", 0)
	STRUCT_FIELD(std::string, KeyState_bufferName, "", "256 entries for the keys this frame, and 256 entries for the keys last frame. 512 entries total.", 0)
STRUCT_END()

STRUCT_BEGIN(GGUserFile_SavedVariable, "Saved Variable Values")
	STRUCT_FIELD(std::string, name, "", "", 0)
	STRUCT_FIELD(std::string, value, "", "", 0)
STRUCT_END()

STRUCT_BEGIN(GGUserFileV1, "The contents of a .gguser file")
	STRUCT_FIELD(std::string, version, "1.0", "The version of the .gguser file", SCHEMA_FLAG_SERIALIZE_DFLT)
	STRUCT_FIELD(GGUserFile_SystemVars, systemVars, {}, "", 0)
	STRUCT_FIELD(int, resourceViewType, 0, "The type of resource being viewed", 0)
	STRUCT_FIELD(int, resourceViewNodeIndex, -1, "The index of the node bieng viewed", 0)
	STRUCT_FIELD(int, resourceViewResourceIndex, -1, "The index of that resource within that node being used", 0)
	STRUCT_FIELD(int, syncInterval, 1, "IDXGISwapChain::Present() parameter: Synchronize presentation after the nth vertical blank.", 0)
	STRUCT_DYNAMIC_ARRAY(GGUserFile_ImportedResource, importedResources, "", 0)
	STRUCT_DYNAMIC_ARRAY(GGUserFile_SavedVariable, savedVariables, "", 0)
STRUCT_END()

STRUCT_BEGIN(GGUserFileV2Snapshot, "The snapshot of a GGUserFileV2")
	STRUCT_FIELD(std::string, name, "", "The snapshot name", 0)
	STRUCT_FIELD(int, resourceViewType, 0, "The type of resource being viewed", 0)
	STRUCT_FIELD(int, resourceViewNodeIndex, -1, "The index of the node bieng viewed", 0)
	STRUCT_FIELD(int, resourceViewResourceIndex, -1, "The index of that resource within that node being used", 0)

	STRUCT_FIELD(bool, loadVars, true, "Whether variables will be loaded from this snapshot", 0)
	STRUCT_FIELD(bool, loadCamera, true, "Whether the camera will be loaded from this snapshot", 0)
	STRUCT_FIELD(bool, loadResources, true, "Whether imported resources will be loaded from this snapshot", 0)
	STRUCT_FIELD(bool, loadView, true, "Whether the resource viewed will be loaded from this snapshot", 0)

	STRUCT_STATIC_ARRAY(float, cameraPos, 3, { 0.0f COMMA 0.0f COMMA - 10.0f }, "Used by snapshots to capture the camera position", 0)
	STRUCT_STATIC_ARRAY(float, cameraAltitudeAzimuth, 2, { 0.0f COMMA 0.0f }, "Used by snapshots to capture the camera orientation", 0)
	STRUCT_DYNAMIC_ARRAY(GGUserFile_ImportedResource, importedResources, "", 0)
	STRUCT_DYNAMIC_ARRAY(GGUserFile_SavedVariable, savedVariables, "", 0)
STRUCT_END()

STRUCT_BEGIN(GGUserFileV2, "The contents of a .gguser file")
	STRUCT_FIELD(std::string, version, "2.0", "The version of the .gguser file", SCHEMA_FLAG_SERIALIZE_DFLT)
	STRUCT_FIELD(int, syncInterval, 1, "IDXGISwapChain::Present() parameter: Synchronize presentation after the nth vertical blank.", 0)
	STRUCT_FIELD(GGUserFile_SystemVars, systemVars, {}, "", 0)
	STRUCT_FIELD(GGUserFileV2Snapshot, snapshot, {}, "", 0)
	STRUCT_DYNAMIC_ARRAY(GGUserFileV2Snapshot, snapshots, "", 0)
STRUCT_END()

STRUCT_BEGIN(GGUserFileVersionOnly, "Only the version of the .gguser file")
	STRUCT_FIELD(std::string, version, "1.0", "The version of the .gguser file", SCHEMA_FLAG_SERIALIZE_DFLT)
STRUCT_END()
