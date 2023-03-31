// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Someday/SomedayGameMode.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeSomedayGameMode() {}
// Cross Module References
	SOMEDAY_API UClass* Z_Construct_UClass_ASomedayGameMode_NoRegister();
	SOMEDAY_API UClass* Z_Construct_UClass_ASomedayGameMode();
	ENGINE_API UClass* Z_Construct_UClass_AGameModeBase();
	UPackage* Z_Construct_UPackage__Script_Someday();
// End Cross Module References
	void ASomedayGameMode::StaticRegisterNativesASomedayGameMode()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(ASomedayGameMode);
	UClass* Z_Construct_UClass_ASomedayGameMode_NoRegister()
	{
		return ASomedayGameMode::StaticClass();
	}
	struct Z_Construct_UClass_ASomedayGameMode_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_ASomedayGameMode_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_AGameModeBase,
		(UObject* (*)())Z_Construct_UPackage__Script_Someday,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ASomedayGameMode_Statics::Class_MetaDataParams[] = {
		{ "HideCategories", "Info Rendering MovementReplication Replication Actor Input Movement Collision Rendering HLOD WorldPartition DataLayers Transformation" },
		{ "IncludePath", "SomedayGameMode.h" },
		{ "ModuleRelativePath", "SomedayGameMode.h" },
		{ "ShowCategories", "Input|MouseInput Input|TouchInput" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_ASomedayGameMode_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<ASomedayGameMode>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_ASomedayGameMode_Statics::ClassParams = {
		&ASomedayGameMode::StaticClass,
		"Game",
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		nullptr,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		0,
		0,
		0x008802ACu,
		METADATA_PARAMS(Z_Construct_UClass_ASomedayGameMode_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_ASomedayGameMode_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_ASomedayGameMode()
	{
		if (!Z_Registration_Info_UClass_ASomedayGameMode.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_ASomedayGameMode.OuterSingleton, Z_Construct_UClass_ASomedayGameMode_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_ASomedayGameMode.OuterSingleton;
	}
	template<> SOMEDAY_API UClass* StaticClass<ASomedayGameMode>()
	{
		return ASomedayGameMode::StaticClass();
	}
	DEFINE_VTABLE_PTR_HELPER_CTOR(ASomedayGameMode);
	struct Z_CompiledInDeferFile_FID_Someday_Source_Someday_SomedayGameMode_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Someday_Source_Someday_SomedayGameMode_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_ASomedayGameMode, ASomedayGameMode::StaticClass, TEXT("ASomedayGameMode"), &Z_Registration_Info_UClass_ASomedayGameMode, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(ASomedayGameMode), 645629342U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Someday_Source_Someday_SomedayGameMode_h_592068871(TEXT("/Script/Someday"),
		Z_CompiledInDeferFile_FID_Someday_Source_Someday_SomedayGameMode_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Someday_Source_Someday_SomedayGameMode_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
