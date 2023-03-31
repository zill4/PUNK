// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeSomeday_init() {}
	static FPackageRegistrationInfo Z_Registration_Info_UPackage__Script_Someday;
	FORCENOINLINE UPackage* Z_Construct_UPackage__Script_Someday()
	{
		if (!Z_Registration_Info_UPackage__Script_Someday.OuterSingleton)
		{
			static const UECodeGen_Private::FPackageParams PackageParams = {
				"/Script/Someday",
				nullptr,
				0,
				PKG_CompiledIn | 0x00000000,
				0x8699DD4B,
				0xF173372C,
				METADATA_PARAMS(nullptr, 0)
			};
			UECodeGen_Private::ConstructUPackage(Z_Registration_Info_UPackage__Script_Someday.OuterSingleton, PackageParams);
		}
		return Z_Registration_Info_UPackage__Script_Someday.OuterSingleton;
	}
	static FRegisterCompiledInInfo Z_CompiledInDeferPackage_UPackage__Script_Someday(Z_Construct_UPackage__Script_Someday, TEXT("/Script/Someday"), Z_Registration_Info_UPackage__Script_Someday, CONSTRUCT_RELOAD_VERSION_INFO(FPackageReloadVersionInfo, 0x8699DD4B, 0xF173372C));
PRAGMA_ENABLE_DEPRECATION_WARNINGS
