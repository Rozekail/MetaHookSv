#include "PhysicUTIL.h"
#include "Controls.h"
#include "exportfuncs.h"

#include <sstream>
#include <format>
#include <algorithm>

const char* VGUI2Token_PhysicObjectType[] = { "#BulletPhysics_None", "#BulletPhysics_StaticObject", "#BulletPhysics_DynamicObject", "#BulletPhysics_RagdollObject" };

const char* UTIL_GetPhysicObjectTypeLocalizationToken(int type)
{
	if (type >= 0 && type < _ARRAYSIZE(VGUI2Token_PhysicObjectType))
	{
		return VGUI2Token_PhysicObjectType[type];
	}

	return "#BulletPhysics_None";
}

std::wstring UTIL_GetPhysicObjectTypeLocalizedName(int type)
{
	return vgui::localize()->Find(UTIL_GetPhysicObjectTypeLocalizationToken(type));
}

const char* VGUI2Token_ConstraintType[] = { 
	"#BulletPhysics_None", 
	"#BulletPhysics_ConeTwistConstraint", 
	"#BulletPhysics_HingeConstraint",
	"#BulletPhysics_PointConstraint", 
	"#BulletPhysics_SliderConstraint",
	"#BulletPhysics_Dof6Constraint", 
	"#BulletPhysics_Dof6SpringConstraint",
	"#BulletPhysics_FixedConstraint" };

const char* UTIL_GetConstraintTypeLocalizationToken(int type)
{
	if (type >= 0 && type < _ARRAYSIZE(VGUI2Token_ConstraintType))
	{
		return VGUI2Token_ConstraintType[type];
	}

	return "#BulletPhysics_None";
}

std::wstring UTIL_GetConstraintTypeLocalizedName(int type)
{
	return vgui::localize()->Find(UTIL_GetConstraintTypeLocalizationToken(type));
}

const char* VGUI2Token_RotOrderType[] = { 
	"#BulletPhysics_PhysicRotOrder_XYZ",
	"#BulletPhysics_PhysicRotOrder_XZY",
	"#BulletPhysics_PhysicRotOrder_YXZ", 
	"#BulletPhysics_PhysicRotOrder_YZX",
	"#BulletPhysics_PhysicRotOrder_ZXY",
	"#BulletPhysics_PhysicRotOrder_ZYX" };

const char* UTIL_GetRotOrderTypeLocalizationToken(int type)
{
	if (type >= 0 && type < _ARRAYSIZE(VGUI2Token_RotOrderType))
	{
		return VGUI2Token_RotOrderType[type];
	}

	return "#BulletPhysics_PhysicRotOrder_XYZ";
}

std::wstring UTIL_GetRotOrderTypeLocalizedName(int type)
{
	return vgui::localize()->Find(UTIL_GetRotOrderTypeLocalizationToken(type));
}

const char* VGUI2Token_CollisionShape[] = {
	"#BulletPhysics_None",
	"#BulletPhysics_Box", 
	"#BulletPhysics_Sphere",
	"#BulletPhysics_Capsule",
	"#BulletPhysics_Cylinder", 
	"#BulletPhysics_MultiSphere",
	"#BulletPhysics_TriangleMesh",
	"#BulletPhysics_Compound" };

const char* UTIL_GetCollisionShapeTypeLocalizationToken(int type)
{
	if (type >= 0 && type < _ARRAYSIZE(VGUI2Token_CollisionShape))
	{
		return VGUI2Token_CollisionShape[type];
	}

	return "#BulletPhysics_None";
}

std::wstring UTIL_GetCollisionShapeTypeLocalizedName(int type)
{
	return vgui::localize()->Find(UTIL_GetCollisionShapeTypeLocalizationToken(type));
}

const char* VGUI2Token_PhysicBehaviorType[] = { 
	"#BulletPhysics_None",
	"#BulletPhysics_BarnacleDragOnRigidBody",
	"#BulletPhysics_BarnacleDragOnConstraint",
	"#BulletPhysics_BarnacleChew", 
	"#BulletPhysics_BarnacleConstraintLimitAdjustment",
	"#BulletPhysics_GargantuaDragOnConstraint",
	"#BulletPhysics_FirstPersonViewCamera", 
	"#BulletPhysics_ThirdPersonViewCamera", 
	"#BulletPhysics_SimpleBuoyancy",
	"#BulletPhysics_RigidBodyRelocation",
};

const char* UTIL_GetPhysicBehaviorTypeLocalizationToken(int type)
{
	if (type >= 0 && type < _ARRAYSIZE(VGUI2Token_PhysicBehaviorType))
	{
		return VGUI2Token_PhysicBehaviorType[type];
	}

	return "#BulletPhysics_None";
}

std::wstring UTIL_GetPhysicBehaviorTypeLocalizedName(int type)
{
	return vgui::localize()->Find(UTIL_GetPhysicBehaviorTypeLocalizationToken(type));
}

const char* VGUI2Token_ActivityType[] = { 
	"#BulletPhysics_Idle",
	"#BulletPhysics_Death",
	"#BulletPhysics_CaughtByBarnacle", 
	"#BulletPhysics_BarnaclePulling",
	"#BulletPhysics_BarnacleChewing",
	"#BulletPhysics_GargantuaBite",
	"#BulletPhysics_Debug"
};

const char* UTIL_GetActivityTypeLocalizationToken(StudioAnimActivityType type)
{
	if (type >= 0 && type < _ARRAYSIZE(VGUI2Token_ActivityType))
	{
		return VGUI2Token_ActivityType[type];
	}

	return "#BulletPhysics_Idle";
}

std::wstring UTIL_GetActivityTypeLocalizedName(StudioAnimActivityType type)
{
	return vgui::localize()->Find(UTIL_GetActivityTypeLocalizationToken(type));
}

std::wstring UTIL_GetFormattedRigidBodyFlags(int flags)
{
	std::wstringstream ss;

	// Macro to format flag string
#define FORMAT_FLAGS_TO_STRING(name) if (flags & PhysicRigidBodyFlag_##name) {\
	auto wszLocalizedString = vgui::localize()->Find("#BulletPhysics_" #name);\
		if (wszLocalizedString) {\
			ss << L"(" << wszLocalizedString << L") "; \
		}\
		else\
		{\
			ss << L"(" << (#name) << L") "; \
		}\
    }

	FORMAT_FLAGS_TO_STRING(AlwaysDynamic);
	FORMAT_FLAGS_TO_STRING(AlwaysKinematic);
	FORMAT_FLAGS_TO_STRING(AlwaysStatic);
	FORMAT_FLAGS_TO_STRING(InvertStateOnIdle);
	FORMAT_FLAGS_TO_STRING(InvertStateOnDeath);
	FORMAT_FLAGS_TO_STRING(InvertStateOnCaughtByBarnacle);
	FORMAT_FLAGS_TO_STRING(InvertStateOnBarnaclePulling);
	FORMAT_FLAGS_TO_STRING(InvertStateOnBarnacleChewing);
	FORMAT_FLAGS_TO_STRING(InvertStateOnGargantuaBite);
	FORMAT_FLAGS_TO_STRING(NoCollisionToWorld);
	FORMAT_FLAGS_TO_STRING(NoCollisionToStaticObject);
	FORMAT_FLAGS_TO_STRING(NoCollisionToDynamicObject);
	FORMAT_FLAGS_TO_STRING(NoCollisionToRagdollObject);

#undef FORMAT_FLAGS_TO_STRING

	return ss.str();
}

std::wstring UTIL_GetFormattedConstraintFlags(int flags)
{
	std::wstringstream ss;

	// Macro to format flag string
#define FORMAT_FLAGS_TO_STRING(name) if (flags & PhysicConstraintFlag_##name) {\
	auto wszLocalizedString = vgui::localize()->Find("#BulletPhysics_" #name);\
		if (wszLocalizedString) {\
			ss << L"(" << wszLocalizedString << L") "; \
		}\
		else\
		{\
			ss << L"(" << (#name) << L") "; \
		}\
    }

	FORMAT_FLAGS_TO_STRING(Barnacle);
	FORMAT_FLAGS_TO_STRING(Gargantua);
	FORMAT_FLAGS_TO_STRING(DeactiveOnNormalActivity);
	FORMAT_FLAGS_TO_STRING(DeactiveOnDeathActivity);
	FORMAT_FLAGS_TO_STRING(DeactiveOnCaughtByBarnacleActivity);
	FORMAT_FLAGS_TO_STRING(DeactiveOnBarnaclePullingActivity);
	FORMAT_FLAGS_TO_STRING(DeactiveOnBarnacleChewingActivity);
	FORMAT_FLAGS_TO_STRING(DeactiveOnGargantuaBiteActivity);
	FORMAT_FLAGS_TO_STRING(DontResetPoseOnErrorCorrection);
	FORMAT_FLAGS_TO_STRING(DeferredCreate);

#undef FORMAT_FLAGS_TO_STRING

	return ss.str();
}

std::wstring UTIL_GetFormattedConstraintConfigAttributes(const CClientConstraintConfig *pConstraintConfig)
{
	std::wstringstream ss;

	if (pConstraintConfig->useGlobalJointFromA)
	{
		ss << std::format(L"({0}) ", vgui::localize()->Find("#BulletPhysics_UseGlobalJointFromA"));
	}
	else
	{
		ss << std::format(L"({0}) ", vgui::localize()->Find("#BulletPhysics_UseGlobalJointFromB"));
	}

	if (pConstraintConfig->disableCollision)
	{
		ss << std::format(L"({0}) ", vgui::localize()->Find("#BulletPhysics_DisableCollision"));
	}
	else
	{
		ss << std::format(L"({0}) ", vgui::localize()->Find("#BulletPhysics_DontDisableCollision"));
	}

	if (pConstraintConfig->useLinearReferenceFrameA)
	{
		ss << std::format(L"({0}) ", vgui::localize()->Find("#BulletPhysics_UseLinearReferenceFrameA"));
	}
	else
	{
		ss << std::format(L"({0}) ", vgui::localize()->Find("#BulletPhysics_UseLinearReferenceFrameB"));
	}

	if (pConstraintConfig->useLookAtOther)
	{
		ss << std::format(L"({0}) ", vgui::localize()->Find("#BulletPhysics_UseLookAtOther"));
	}

	if (pConstraintConfig->useGlobalJointOriginFromOther)
	{
		ss << std::format(L"({0}) ", vgui::localize()->Find("#BulletPhysics_UseGlobalJointOriginFromOther"));
	}

	if (pConstraintConfig->useRigidBodyDistanceAsLinearLimit)
	{
		ss << std::format(L"({0}) ", vgui::localize()->Find("#BulletPhysics_UseRigidBodyDistanceAsLinearLimit"));
	}

	if (pConstraintConfig->useSeperateLocalFrame)
	{
		ss << std::format(L"({0}) ", vgui::localize()->Find("#BulletPhysics_UseSeperateLocalFrame"));
	}

	return ss.str();
}

std::wstring UTIL_GetFormattedPhysicBehaviorFlags(int flags)
{
	std::wstringstream ss;

	// Macro to format flag string
#define FORMAT_FLAGS_TO_STRING(name) if (flags & PhysicBehaviorFlag_##name) {\
	auto wszLocalizedString = vgui::localize()->Find("#BulletPhysics_" #name);\
		if (wszLocalizedString) {\
			ss << L"(" << wszLocalizedString << L") "; \
		}\
		else\
		{\
			ss << L"(" << (#name) << L") "; \
		}\
    }

	FORMAT_FLAGS_TO_STRING(Barnacle);
	FORMAT_FLAGS_TO_STRING(Gargantua);

#undef FORMAT_FLAGS_TO_STRING

	return ss.str();
}

std::wstring UTIL_GetFormattedAnimControlFlags(int flags)
{
	std::wstringstream ss;

	// Macro to format flag string
#define FORMAT_FLAGS_TO_STRING(name) if (flags & AnimControlFlag_##name) {\
	auto wszLocalizedString = vgui::localize()->Find("#BulletPhysics_" #name);\
		if (wszLocalizedString) {\
			ss << L"(" << wszLocalizedString << L") "; \
		}\
		else\
		{\
			ss << L"(" << (#name) << L") "; \
		}\
    }

	FORMAT_FLAGS_TO_STRING(OverrideAllBones);
	FORMAT_FLAGS_TO_STRING(OverrideController);
	FORMAT_FLAGS_TO_STRING(OverrideBlending);

#undef FORMAT_FLAGS_TO_STRING

	return ss.str();
}

const char* UTIL_GetSequenceRawName(studiohdr_t* studiohdr, int sequence)
{
	if (!(sequence >= 0 && sequence < studiohdr->numseq))
	{
		return "--";
	}

	auto pseqdesc = (mstudioseqdesc_t*)((byte*)studiohdr + studiohdr->seqindex) + sequence;

	if (pseqdesc->seqgroup == 0)
	{
		return pseqdesc->label;
	}

	auto pseqgroup = (mstudioseqgroup_t*)((byte*)studiohdr + studiohdr->seqgroupindex) + pseqdesc->seqgroup;

	return pseqgroup->label;
}

std::string UTIL_GetFormattedSequenceNameEx(studiohdr_t* studiohdr, int sequence)
{
	if (!(sequence >= 0 && sequence < studiohdr->numseq))
	{
		return "--";
	}

	return std::format("#{0} ({1})", sequence, UTIL_GetSequenceRawName(studiohdr, sequence));
}

std::string UTIL_GetFormattedSequenceName(int modelindex, int sequence)
{
	auto model = EngineGetModelByIndex(modelindex);

	if (!model)
	{
		return "--";
	}

	auto studiohdr = (studiohdr_t*)IEngineStudio.Mod_Extradata(model);

	if (!studiohdr)
	{
		return "--";
	}

	return UTIL_GetFormattedSequenceNameEx(studiohdr, sequence);
}

const char* UTIL_GetBoneRawName(studiohdr_t* studiohdr, int boneindex)
{
	if (!(boneindex >= 0 && boneindex < studiohdr->numbones))
	{
		return "--";
	}

	auto pbone = (mstudiobone_t*)((byte*)studiohdr + studiohdr->boneindex);

	return pbone[boneindex].name;
}

std::string UTIL_GetFormattedBoneNameEx(studiohdr_t* studiohdr, int boneindex)
{
	if (!(boneindex >= 0 && boneindex < studiohdr->numbones))
	{
		return "--";
	}

	return std::format("#{0} ({1})", boneindex, UTIL_GetBoneRawName(studiohdr, boneindex));
}

std::string UTIL_GetFormattedBoneName(int modelindex, int boneindex)
{
	auto model = EngineGetModelByIndex(modelindex);

	if (!model)
	{
		return "--";
	}

	auto studiohdr = (studiohdr_t*)IEngineStudio.Mod_Extradata(model);

	if (!studiohdr)
	{
		return "--";
	}

	return UTIL_GetFormattedBoneNameEx(studiohdr, boneindex);
}

std::string UTIL_GetAbsoluteModelName(model_t* mod)
{
	if (mod->type == mod_brush)
	{
		if (mod->name[0] == '*')
		{
			auto worldmodel = EngineFindWorldModelBySubModel(mod);

			if (worldmodel)
			{
				return std::format("{0}/{1}", worldmodel->name, mod->name);
			}
		}
	}

	return mod->name;
}

const char* UTIL_GetPhysicObjectConfigTypeName(int type)
{
	const char* c_names[] = {
		"None", 
		"StaticObject", 
		"DynamicObject",
		"RagdollObject" 
	};

	if (type >= 0 && type < _ARRAYSIZE(c_names))
	{
		return c_names[type];
	}

	return "Unknown";
}

const char* UTIL_GetConstraintTypeName(int type)
{
	const char* c_names[] = { 
		"None",
		"ConeTwist",
		"Hinge",
		"Point", 
		"Slider", 
		"Dof6", 
		"Dof6Spring",
		"Fixed" };

	if (type >= 0 && type < _ARRAYSIZE(c_names))
	{
		return c_names[type];
	}

	return "Unknown";
}

const char* UTIL_GetPhysicBehaviorTypeName(int type)
{
	const char* c_names[] = { 
		"None", 
		"BarnacleDragOnRigidBody",
		"BarnacleDragOnConstraint",
		"BarnacleChew",
		"BarnacleConstraintLimitAdjustment",
		"GargantuaDragOnConstraint",
		"FirstPersonViewCamera", 
		"ThirdPersonViewCamera", 
		"SimpleBuoyancy",
		"RigidBodyRelocation",
	};

	if (type >= 0 && type < _ARRAYSIZE(c_names))
	{
		return c_names[type];
	}

	return "Unknown";
}

const char* UTIL_GetCollisionShapeTypeName(int type)
{
	const char* c_names[] = { 
		"None",
		"Box", 
		"Sphere",
		"Capsule",
		"Cylinder", 
		"MultiSphere", 
		"TriangleMesh",
		"Compound" };

	if (type >= 0 && type < _ARRAYSIZE(c_names))
	{
		return c_names[type];
	}

	return "Unknown";
}

int UTIL_GetCollisionTypeFromTypeName(const char* name)
{
#define CHECK_COLLISION_SHAPE_TYPE_NAME(ty) if (!strcmp(name, #ty ))\
	{\
		return PhysicShape_##ty;\
	}
	CHECK_COLLISION_SHAPE_TYPE_NAME(Box);
	CHECK_COLLISION_SHAPE_TYPE_NAME(Sphere);
	CHECK_COLLISION_SHAPE_TYPE_NAME(Capsule);
	CHECK_COLLISION_SHAPE_TYPE_NAME(Cylinder);
	CHECK_COLLISION_SHAPE_TYPE_NAME(MultiSphere);
	CHECK_COLLISION_SHAPE_TYPE_NAME(TriangleMesh);
	CHECK_COLLISION_SHAPE_TYPE_NAME(Compound);

	return PhysicShape_None;
}

int UTIL_GetConstraintTypeFromTypeName(const char* name)
{
#define CHECK_CONSTRAINT_TYPE_NAME(ty) if (!strcmp(name, #ty ))\
	{\
		return PhysicConstraint_##ty;\
	}
	CHECK_CONSTRAINT_TYPE_NAME(ConeTwist);
	CHECK_CONSTRAINT_TYPE_NAME(Hinge);
	CHECK_CONSTRAINT_TYPE_NAME(Point);
	CHECK_CONSTRAINT_TYPE_NAME(Slider);
	CHECK_CONSTRAINT_TYPE_NAME(Dof6);
	CHECK_CONSTRAINT_TYPE_NAME(Dof6Spring);
	CHECK_CONSTRAINT_TYPE_NAME(Fixed);

	return PhysicConstraint_None;
}

int UTIL_GetPhysicBehaviorTypeFromTypeName(const char* name)
{
#define CHECK_BEHAVIOR_TYPE_NAME(ty) if (!strcmp(name, #ty ))\
	{\
		return PhysicBehavior_##ty;\
	}
	
	CHECK_BEHAVIOR_TYPE_NAME(BarnacleDragOnRigidBody);
	CHECK_BEHAVIOR_TYPE_NAME(BarnacleDragOnConstraint);
	CHECK_BEHAVIOR_TYPE_NAME(BarnacleChew);
	CHECK_BEHAVIOR_TYPE_NAME(BarnacleConstraintLimitAdjustment);
	CHECK_BEHAVIOR_TYPE_NAME(GargantuaDragOnConstraint);
	CHECK_BEHAVIOR_TYPE_NAME(FirstPersonViewCamera);
	CHECK_BEHAVIOR_TYPE_NAME(ThirdPersonViewCamera);
	CHECK_BEHAVIOR_TYPE_NAME(SimpleBuoyancy);
	CHECK_BEHAVIOR_TYPE_NAME(RigidBodyRelocation);

#undef CHECK_BEHAVIOR_TYPE_NAME

	return PhysicBehavior_None;
}

std::shared_ptr<CClientRigidBodyConfig> UTIL_GetRigidConfigFromConfigId(int configId)
{
	auto pPhysicConfig = ClientPhysicManager()->GetPhysicConfig(configId);

	auto pLockedPhysicConfig = pPhysicConfig.lock();

	if (!pLockedPhysicConfig)
		return nullptr;

	if (pLockedPhysicConfig->configType != PhysicConfigType_RigidBody)
		return nullptr;

	std::shared_ptr<CClientRigidBodyConfig> pRigidBodyConfig = std::static_pointer_cast<CClientRigidBodyConfig>(pLockedPhysicConfig);

	return pRigidBodyConfig;
}

std::shared_ptr<CClientConstraintConfig> UTIL_GetConstraintConfigFromConfigId(int configId)
{
	auto pPhysicConfig = ClientPhysicManager()->GetPhysicConfig(configId);

	auto pLockedPhysicConfig = pPhysicConfig.lock();

	if (!pLockedPhysicConfig)
		return nullptr;

	if (pLockedPhysicConfig->configType != PhysicConfigType_Constraint)
		return nullptr;

	std::shared_ptr<CClientConstraintConfig> pConstraintConfig = std::static_pointer_cast<CClientConstraintConfig>(pLockedPhysicConfig);

	return pConstraintConfig;
}

std::shared_ptr<CClientPhysicBehaviorConfig> UTIL_GetPhysicBehaviorConfigFromConfigId(int configId)
{
	auto pPhysicConfig = ClientPhysicManager()->GetPhysicConfig(configId);

	auto pLockedPhysicConfig = pPhysicConfig.lock();

	if (!pLockedPhysicConfig)
		return nullptr;

	if (pLockedPhysicConfig->configType != PhysicConfigType_PhysicBehavior)
		return nullptr;

	std::shared_ptr<CClientPhysicBehaviorConfig> pPhysicBehaviorConfig = std::static_pointer_cast<CClientPhysicBehaviorConfig>(pLockedPhysicConfig);

	return pPhysicBehaviorConfig;
}

std::shared_ptr<CClientAnimControlConfig> UTIL_GetAnimControlConfigFromConfigId(int configId)
{
	auto pPhysicConfig = ClientPhysicManager()->GetPhysicConfig(configId);

	auto pLockedPhysicConfig = pPhysicConfig.lock();

	if (!pLockedPhysicConfig)
		return nullptr;

	if (pLockedPhysicConfig->configType != PhysicConfigType_AnimControl)
		return nullptr;

	std::shared_ptr<CClientAnimControlConfig> pAnimControlConfig = std::static_pointer_cast<CClientAnimControlConfig>(pLockedPhysicConfig);

	return pAnimControlConfig;
}

std::shared_ptr<CClientPhysicObjectConfig> UTIL_GetPhysicObjectConfigFromConfigId(int configId)
{
	auto pPhysicConfig = ClientPhysicManager()->GetPhysicConfig(configId);

	auto pLockedPhysicConfig = pPhysicConfig.lock();

	if (!pLockedPhysicConfig)
		return nullptr;

	if (pLockedPhysicConfig->configType != PhysicConfigType_PhysicObject)
		return nullptr;

	std::shared_ptr<CClientPhysicObjectConfig> pPhysicObjectConfig = std::static_pointer_cast<CClientPhysicObjectConfig>(pLockedPhysicConfig);

	return pPhysicObjectConfig;
}

std::shared_ptr<CClientRagdollObjectConfig> UTIL_ConvertPhysicObjectConfigToRagdollObjectConfig(const std::shared_ptr<CClientPhysicObjectConfig> &p)
{
	if (p->type != PhysicObjectType_RagdollObject)
		return nullptr;

	std::shared_ptr<CClientRagdollObjectConfig> pConverted = std::static_pointer_cast<CClientRagdollObjectConfig>(p);

	return pConverted;
}

std::shared_ptr<CClientDynamicObjectConfig> UTIL_ConvertPhysicObjectConfigToDynamicObjectConfig(const std::shared_ptr<CClientPhysicObjectConfig>& p)
{
	if (p->type != PhysicObjectType_DynamicObject)
		return nullptr;

	std::shared_ptr<CClientDynamicObjectConfig> pConverted = std::static_pointer_cast<CClientDynamicObjectConfig>(p);

	return pConverted;
}

std::shared_ptr<CClientStaticObjectConfig> UTIL_ConvertPhysicObjectConfigToStaticObjectConfig(const std::shared_ptr<CClientPhysicObjectConfig>& p)
{
	if (p->type != PhysicObjectType_StaticObject)
		return nullptr;

	std::shared_ptr<CClientStaticObjectConfig> pConverted = std::static_pointer_cast<CClientStaticObjectConfig>(p);

	return pConverted;
}

bool UTIL_RemoveRigidBodyFromPhysicObjectConfig(CClientPhysicObjectConfig * pPhysicObjectConfig, int configId)
{
	for (auto itor = pPhysicObjectConfig->RigidBodyConfigs.begin(); itor != pPhysicObjectConfig->RigidBodyConfigs.end(); )
	{
		const auto& p = (*itor);

		if (p->configId == configId)
		{
			itor = pPhysicObjectConfig->RigidBodyConfigs.erase(itor);

			pPhysicObjectConfig->configModified = true;

			return true;
		}

		itor++;
	}

	return false;
}

bool UTIL_RemoveConstraintFromPhysicObjectConfig(CClientPhysicObjectConfig* pPhysicObjectConfig, int configId)
{
	for (auto itor = pPhysicObjectConfig->ConstraintConfigs.begin(); itor != pPhysicObjectConfig->ConstraintConfigs.end(); )
	{
		const auto& p = (*itor);

		if (p->configId == configId)
		{
			itor = pPhysicObjectConfig->ConstraintConfigs.erase(itor);

			pPhysicObjectConfig->configModified = true;

			return true;
		}

		itor++;
	}

	return false;
}

bool UTIL_RemovePhysicBehaviorFromPhysicObjectConfig(CClientPhysicObjectConfig* pPhysicObjectConfig, int configId)
{
	for (auto itor = pPhysicObjectConfig->PhysicBehaviorConfigs.begin(); itor != pPhysicObjectConfig->PhysicBehaviorConfigs.end(); )
	{
		const auto& p = (*itor);

		if (p->configId == configId)
		{
			itor = pPhysicObjectConfig->PhysicBehaviorConfigs.erase(itor);

			pPhysicObjectConfig->configModified = true;

			return true;
		}

		itor++;
	}

	return false;
}

bool UTIL_RemoveAnimControlFromRagdollObjectConfig(CClientRagdollObjectConfig* pRagdollObjectConfig, int configId)
{
	for (auto itor = pRagdollObjectConfig->AnimControlConfigs.begin(); itor != pRagdollObjectConfig->AnimControlConfigs.end(); )
	{
		const auto& p = (*itor);

		if (p->configId == configId)
		{
			itor = pRagdollObjectConfig->AnimControlConfigs.erase(itor);

			pRagdollObjectConfig->configModified = true;

			return true;
		}

		itor++;
	}

	return false;
}

std::shared_ptr<CClientCollisionShapeConfig> UTIL_CreateEmptyCollisionShapeConfig()
{
	auto pNewShape = std::make_shared<CClientCollisionShapeConfig>();

	pNewShape->type = PhysicShape_Sphere;
	pNewShape->size[0] = 3;
	pNewShape->configModified = true;

	ClientPhysicManager()->AddPhysicConfig(pNewShape->configId, pNewShape);

	return pNewShape;
}

std::shared_ptr<CClientRigidBodyConfig> UTIL_CreateEmptyRigidBodyConfig()
{
	auto pNewConfig = std::make_shared<CClientRigidBodyConfig>();

	pNewConfig->name = std::format("UnnamedRigidBody_{0}", pNewConfig->configId);
	pNewConfig->configModified = true;

	pNewConfig->collisionShape = UTIL_CreateEmptyCollisionShapeConfig();

	ClientPhysicManager()->AddPhysicConfig(pNewConfig->configId, pNewConfig);

	return pNewConfig;
}

std::shared_ptr<CClientConstraintConfig> UTIL_CreateEmptyConstraintConfig()
{
	auto pNewConfig = std::make_shared<CClientConstraintConfig>();

	pNewConfig->name = std::format("UnnamedConstraint_{0}", pNewConfig->configId);
	pNewConfig->configModified = true;

	ClientPhysicManager()->AddPhysicConfig(pNewConfig->configId, pNewConfig);

	return pNewConfig;
}

std::shared_ptr<CClientPhysicBehaviorConfig> UTIL_CreateEmptyPhysicBehaviorConfig()
{
	auto pNewConfig = std::make_shared<CClientPhysicBehaviorConfig>();

	pNewConfig->name = std::format("UnnamedPhysicBehavior_{0}", pNewConfig->configId);
	pNewConfig->configModified = true;

	ClientPhysicManager()->AddPhysicConfig(pNewConfig->configId, pNewConfig);

	return pNewConfig;
}

std::shared_ptr<CClientAnimControlConfig> UTIL_CreateEmptyAnimControlConfig()
{
	auto pNewConfig = std::make_shared<CClientAnimControlConfig>();

	pNewConfig->configModified = true;

	ClientPhysicManager()->AddPhysicConfig(pNewConfig->configId, pNewConfig);

	return pNewConfig;
}

std::shared_ptr<CClientCollisionShapeConfig> UTIL_CloneCollisionShapeConfig(const CClientCollisionShapeConfig* pOldShape)
{
	auto pNewShape = std::make_shared<CClientCollisionShapeConfig>();

	pNewShape->configModified = true;

	pNewShape->configType = pOldShape->configType;
	pNewShape->type = pOldShape->type;
	pNewShape->direction = pOldShape->direction;
	VectorCopy(pOldShape->size, pNewShape->size);
	pNewShape->is_child = pOldShape->is_child;
	VectorCopy(pOldShape->origin, pNewShape->origin);
	VectorCopy(pOldShape->angles, pNewShape->angles);
	//pNewShape->multispheres = pOldShape->multispheres;
	pNewShape->resourcePath = pOldShape->resourcePath;

	for (auto& oldChildShape : pOldShape->compoundShapes) {

		auto pClonedShape = UTIL_CloneCollisionShapeConfig(oldChildShape.get());

		pNewShape->compoundShapes.push_back(pClonedShape);
	}

	ClientPhysicManager()->AddPhysicConfig(pNewShape->configId, pNewShape);

	return pNewShape;
}

std::shared_ptr<CClientRigidBodyConfig> UTIL_CloneRigidBodyConfig(const CClientRigidBodyConfig* pOldConfig)
{
	auto pNewConfig = std::make_shared<CClientRigidBodyConfig>();

	pNewConfig->configModified = true;
	pNewConfig->name = pOldConfig->name;
	pNewConfig->configType = pOldConfig->configType;
	pNewConfig->flags = pOldConfig->flags;
	pNewConfig->debugDrawLevel = pOldConfig->debugDrawLevel;
	pNewConfig->boneindex = pOldConfig->boneindex;
	VectorCopy(pOldConfig->origin, pNewConfig->origin);
	VectorCopy(pOldConfig->angles, pNewConfig->angles);
	pNewConfig->isLegacyConfig = pOldConfig->isLegacyConfig;
	pNewConfig->pboneindex = pOldConfig->pboneindex;
	pNewConfig->pboneoffset = pOldConfig->pboneoffset;
	VectorCopy(pOldConfig->forward, pNewConfig->forward);
	pNewConfig->mass = pOldConfig->mass;
	pNewConfig->density = pOldConfig->density;
	pNewConfig->linearFriction = pOldConfig->linearFriction;
	pNewConfig->rollingFriction = pOldConfig->rollingFriction;
	pNewConfig->restitution = pOldConfig->restitution;
	pNewConfig->ccdRadius = pOldConfig->ccdRadius;
	pNewConfig->ccdThreshold = pOldConfig->ccdThreshold;
	pNewConfig->linearSleepingThreshold = pOldConfig->linearSleepingThreshold;
	pNewConfig->angularSleepingThreshold = pOldConfig->angularSleepingThreshold;

	if (pOldConfig->collisionShape)
	{
		auto pNewShape = UTIL_CloneCollisionShapeConfig(pOldConfig->collisionShape.get());

		pNewConfig->collisionShape = pNewShape;
	}

	ClientPhysicManager()->AddPhysicConfig(pNewConfig->configId, pNewConfig);

	return pNewConfig;
}

std::shared_ptr<CClientConstraintConfig> UTIL_CloneConstraintConfig(const CClientConstraintConfig* pOldConfig)
{
	auto pNewConfig = std::make_shared<CClientConstraintConfig>();

	pNewConfig->configModified = true;

	// Copy basic types and strings
	pNewConfig->name = pOldConfig->name;
	pNewConfig->type = pOldConfig->type;
	pNewConfig->rigidbodyA = pOldConfig->rigidbodyA;
	pNewConfig->rigidbodyB = pOldConfig->rigidbodyB;
	pNewConfig->disableCollision = pOldConfig->disableCollision;
	pNewConfig->useGlobalJointFromA = pOldConfig->useGlobalJointFromA;
	pNewConfig->useLinearReferenceFrameA = pOldConfig->useLinearReferenceFrameA;
	pNewConfig->useLookAtOther = pOldConfig->useLookAtOther;
	pNewConfig->useGlobalJointOriginFromOther = pOldConfig->useGlobalJointOriginFromOther;
	pNewConfig->useRigidBodyDistanceAsLinearLimit = pOldConfig->useRigidBodyDistanceAsLinearLimit;
	pNewConfig->useSeperateLocalFrame = pOldConfig->useSeperateLocalFrame;
	pNewConfig->rotOrder = pOldConfig->rotOrder;
	pNewConfig->flags = pOldConfig->flags;
	pNewConfig->debugDrawLevel = pOldConfig->debugDrawLevel;
	pNewConfig->maxTolerantLinearError = pOldConfig->maxTolerantLinearError;
	pNewConfig->isLegacyConfig = pOldConfig->isLegacyConfig;
	pNewConfig->boneindexA = pOldConfig->boneindexA;
	pNewConfig->boneindexB = pOldConfig->boneindexB;

	// Copy vec3_t using the provided VectorCopy macro
	VectorCopy(pOldConfig->originA, pNewConfig->originA);
	VectorCopy(pOldConfig->anglesA, pNewConfig->anglesA);
	VectorCopy(pOldConfig->originB, pNewConfig->originB);
	VectorCopy(pOldConfig->anglesB, pNewConfig->anglesB);
	VectorCopy(pOldConfig->forward, pNewConfig->forward);
	VectorCopy(pOldConfig->offsetA, pNewConfig->offsetA);
	VectorCopy(pOldConfig->offsetB, pNewConfig->offsetB);

	// Copy array of floats
	std::copy(std::begin(pOldConfig->factors), std::end(pOldConfig->factors), std::begin(pNewConfig->factors));

	ClientPhysicManager()->AddPhysicConfig(pNewConfig->configId, pNewConfig);

	return pNewConfig;
}

std::shared_ptr<CClientPhysicBehaviorConfig> UTIL_ClonePhysicBehaviorConfig(const CClientPhysicBehaviorConfig* pOldConfig)
{
	auto pNewConfig = std::make_shared<CClientPhysicBehaviorConfig>();

	pNewConfig->configModified = true;

	// Copy basic types and strings
	pNewConfig->name = pOldConfig->name;
	pNewConfig->type = pOldConfig->type;
	pNewConfig->rigidbodyA = pOldConfig->rigidbodyA;
	pNewConfig->rigidbodyB = pOldConfig->rigidbodyB;
	pNewConfig->constraint = pOldConfig->constraint;
	pNewConfig->flags = pOldConfig->flags;
	pNewConfig->debugDrawLevel = pOldConfig->debugDrawLevel;

	// Copy vec3_t using the provided VectorCopy macro
	VectorCopy(pOldConfig->origin, pNewConfig->origin);
	VectorCopy(pOldConfig->angles, pNewConfig->angles);

	// Copy array of floats
	std::copy(std::begin(pOldConfig->factors), std::end(pOldConfig->factors), std::begin(pNewConfig->factors));

	ClientPhysicManager()->AddPhysicConfig(pNewConfig->configId, pNewConfig);

	return pNewConfig;
}

std::shared_ptr<CClientAnimControlConfig> UTIL_CloneAnimControlConfig(const CClientAnimControlConfig* pOldConfig)
{
	auto pNewConfig = std::make_shared<CClientAnimControlConfig>();

	pNewConfig->configModified = true;

	pNewConfig->sequence = pOldConfig->sequence;
	pNewConfig->gaitsequence = pOldConfig->gaitsequence;
	pNewConfig->animframe = pOldConfig->animframe;
	pNewConfig->activityType = pOldConfig->activityType;
	pNewConfig->flags = pOldConfig->flags;

	std::copy(std::begin(pOldConfig->controller), std::end(pOldConfig->controller), std::begin(pNewConfig->controller));
	std::copy(std::begin(pOldConfig->blending), std::end(pOldConfig->blending), std::begin(pNewConfig->blending));

	ClientPhysicManager()->AddPhysicConfig(pNewConfig->configId, pNewConfig);

	return pNewConfig;
}

bool UTIL_IsCollisionShapeConfigModified(const CClientCollisionShapeConfig* pCollisionShapeConfig)
{
	if (pCollisionShapeConfig->configModified)
		return true;

	for (const auto& pSubShapeConfig : pCollisionShapeConfig->compoundShapes)
	{
		if (UTIL_IsCollisionShapeConfigModified(pSubShapeConfig.get()))
		{
			return true;
		}
	}

	return false;
}

bool UTIL_IsPhysicObjectConfigModified(const CClientPhysicObjectConfig* pPhysicObjectConfig)
{
	if (pPhysicObjectConfig->configModified)
		return true;

	for (const auto& pRigidBodyConfig : pPhysicObjectConfig->RigidBodyConfigs)
	{
		if (pRigidBodyConfig->configModified)
			return true;

		const auto &pCollisionShapeConfig = pRigidBodyConfig->collisionShape;

		if (pCollisionShapeConfig)
		{
			if(UTIL_IsCollisionShapeConfigModified(pCollisionShapeConfig.get()))
				return true;
		}
	}

	for (const auto& pConstraintConfig : pPhysicObjectConfig->ConstraintConfigs)
	{
		if (pConstraintConfig->configModified)
			return true;
	}

	for (const auto& pPhysicBehaviorConfig : pPhysicObjectConfig->PhysicBehaviorConfigs)
	{
		if (pPhysicBehaviorConfig->configModified)
			return true;
	}

	if (pPhysicObjectConfig->type == PhysicObjectType_RagdollObject)
	{
		const auto pRagdollObjectConfig = (const CClientRagdollObjectConfig*)pPhysicObjectConfig;

		for (const auto& pAnimControl : pRagdollObjectConfig->AnimControlConfigs)
		{
			if (pAnimControl->configModified)
				return true;
		}
	}

	return false;
}

void UTIL_SetCollisionShapeConfigUnmodified(CClientCollisionShapeConfig* pCollisionShapeConfig)
{
	pCollisionShapeConfig->configModified = false;

	for (const auto& pSubShapeConfig : pCollisionShapeConfig->compoundShapes)
	{
		UTIL_SetCollisionShapeConfigUnmodified(pSubShapeConfig.get());
	}
}

void UTIL_SetPhysicObjectConfigUnmodified(CClientPhysicObjectConfig* pPhysicObjectConfig)
{
	pPhysicObjectConfig->configModified = false;

	for (const auto& pRigidBodyConfig : pPhysicObjectConfig->RigidBodyConfigs)
	{
		pRigidBodyConfig->configModified = false;

		const auto& pCollisionShapeConfig = pRigidBodyConfig->collisionShape;

		if (pCollisionShapeConfig)
		{
			UTIL_SetCollisionShapeConfigUnmodified(pCollisionShapeConfig.get());
		}
	}

	for (const auto& pConstraintConfig : pPhysicObjectConfig->ConstraintConfigs)
	{
		pConstraintConfig->configModified = false;
	}

	for (const auto& pPhysicBehaviorConfig : pPhysicObjectConfig->PhysicBehaviorConfigs)
	{
		pPhysicBehaviorConfig->configModified = false;
	}

	if (pPhysicObjectConfig->type == PhysicObjectType_RagdollObject)
	{
		const auto pRagdollObjectConfig = (const CClientRagdollObjectConfig*)pPhysicObjectConfig;

		for (const auto& pAnimControl : pRagdollObjectConfig->AnimControlConfigs)
		{
			pAnimControl->configModified = false;
		}
	}
}

IPhysicRigidBody* UTIL_GetPhysicComponentAsRigidBody(int physicComponentId)
{
	auto pPhysicComponent = ClientPhysicManager()->GetPhysicComponent(physicComponentId);

	if (pPhysicComponent && pPhysicComponent->IsRigidBody())
	{
		return (IPhysicRigidBody*)pPhysicComponent;
	}

	return nullptr;
}

IPhysicConstraint* UTIL_GetPhysicComponentAsConstraint(int physicComponentId)
{
	auto pPhysicComponent = ClientPhysicManager()->GetPhysicComponent(physicComponentId);

	if (pPhysicComponent && pPhysicComponent->IsConstraint())
	{
		return (IPhysicConstraint*)pPhysicComponent;
	}

	return nullptr;
}

IPhysicBehavior* UTIL_GetPhysicComponentAsPhysicBehavior(int physicComponentId)
{
	auto pPhysicComponent = ClientPhysicManager()->GetPhysicComponent(physicComponentId);

	if (pPhysicComponent && pPhysicComponent->IsPhysicBehavior())
	{
		return (IPhysicBehavior*)pPhysicComponent;
	}

	return nullptr;
}

//RigidBody config order related

int UTIL_GetRigidBodyIndex(const CClientPhysicObjectConfig* pPhysicObjectConfig, int configId)
{
	auto& configs = pPhysicObjectConfig->RigidBodyConfigs;

	auto it = std::find_if(configs.begin(), configs.end(), [configId](const std::shared_ptr<CClientRigidBodyConfig>& ptr) {
		return ptr->configId == configId;
	});

	if (it != configs.begin() && it != configs.end()) {
		// Find the index of the current element
		std::size_t currentIndex = std::distance(configs.begin(), it);
		// Calculate the index of the previous element
		return currentIndex;
	}

	return -1;
}

int UTIL_GetRigidBodyIndex(const CClientPhysicObjectConfig* pPhysicObjectConfig, const CClientRigidBodyConfig* pRigidBodyConfig)
{
	auto& configs = pPhysicObjectConfig->RigidBodyConfigs;

	auto it = std::find_if(configs.begin(), configs.end(), [pRigidBodyConfig](const std::shared_ptr<CClientRigidBodyConfig>& ptr) {
		return ptr.get() == pRigidBodyConfig;
		});

	if (it != configs.begin() && it != configs.end()) {
		// Find the index of the current element
		std::size_t currentIndex = std::distance(configs.begin(), it);
		// Calculate the index of the previous element
		return currentIndex;
	}

	return -1;
}

bool UTIL_ShiftUpRigidBodyIndex(CClientPhysicObjectConfig* pPhysicObjectConfig, int configId)
{
	auto& configs = pPhysicObjectConfig->RigidBodyConfigs;

	auto it = std::find_if(configs.begin(), configs.end(), [configId](const std::shared_ptr<CClientRigidBodyConfig>& ptr) {
		return ptr->configId == configId;
		});

	if (it != configs.begin() && it != configs.end()) {
		// Find the index of the current element
		std::size_t currentIndex = std::distance(configs.begin(), it);
		// Calculate the index of the previous element
		std::size_t prevIndex = currentIndex - 1;

		// Swap the current element with the previous one
		std::iter_swap(configs.begin() + currentIndex, configs.begin() + prevIndex);

		return true;
	}

	return false;
}

bool UTIL_ShiftUpRigidBodyIndex(CClientPhysicObjectConfig* pPhysicObjectConfig, CClientRigidBodyConfig* pRigidBodyConfig)
{
	auto& configs = pPhysicObjectConfig->RigidBodyConfigs;

	auto it = std::find_if(configs.begin(), configs.end(), [pRigidBodyConfig](const std::shared_ptr<CClientRigidBodyConfig>& ptr) {
		return ptr.get() == pRigidBodyConfig;
		});

	if (it != configs.begin() && it != configs.end()) {
		// Find the index of the current element
		std::size_t currentIndex = std::distance(configs.begin(), it);
		// Calculate the index of the previous element
		std::size_t prevIndex = currentIndex - 1;

		// Swap the current element with the previous one
		std::iter_swap(configs.begin() + currentIndex, configs.begin() + prevIndex);

		return true;
	}

	return false;
}

bool UTIL_ShiftDownRigidBodyIndex(CClientPhysicObjectConfig* pPhysicObjectConfig, int configId)
{
	auto& configs = pPhysicObjectConfig->RigidBodyConfigs;

	auto it = std::find_if(configs.begin(), configs.end(), [configId](const std::shared_ptr<CClientRigidBodyConfig>& ptr) {
		return ptr->configId == configId;
		});

	if (it != configs.end() - 1 && it != configs.end()) {
		// Find the index of the current element
		std::size_t currentIndex = std::distance(configs.begin(), it);
		// Calculate the index of the next element
		std::size_t nextIndex = currentIndex + 1;

		// Swap the current element with the next one
		std::iter_swap(configs.begin() + currentIndex, configs.begin() + nextIndex);

		return true;
	}

	return false;
}

bool UTIL_ShiftDownRigidBodyIndex(CClientPhysicObjectConfig* pPhysicObjectConfig, CClientRigidBodyConfig* pRigidBodyConfig)
{
	auto& configs = pPhysicObjectConfig->RigidBodyConfigs;

	auto it = std::find_if(configs.begin(), configs.end(), [pRigidBodyConfig](const std::shared_ptr<CClientRigidBodyConfig>& ptr) {
		return ptr.get() == pRigidBodyConfig;
		});

	if (it != configs.end() - 1 && it != configs.end()) {
		// Find the index of the current element
		std::size_t currentIndex = std::distance(configs.begin(), it);
		// Calculate the index of the next element
		std::size_t nextIndex = currentIndex + 1;

		// Swap the current element with the next one
		std::iter_swap(configs.begin() + currentIndex, configs.begin() + nextIndex);

		return true;
	}

	return false;
}

//Constraint config order related

int UTIL_GetConstraintIndex(const CClientPhysicObjectConfig* pPhysicObjectConfig, int configId)
{
	const auto& configs = pPhysicObjectConfig->ConstraintConfigs;

	auto it = std::find_if(configs.begin(), configs.end(), [configId](const std::shared_ptr<CClientConstraintConfig>& ptr) {
		return ptr->configId == configId;
		});

	if (it != configs.end()) {
		return std::distance(configs.begin(), it);
	}

	return -1; // Return -1 if not found
}

int UTIL_GetConstraintIndex(const CClientPhysicObjectConfig* pPhysicObjectConfig, const CClientConstraintConfig* pConstraintConfig)
{
	auto& configs = pPhysicObjectConfig->ConstraintConfigs;

	auto it = std::find_if(configs.begin(), configs.end(), [pConstraintConfig](const std::shared_ptr<CClientConstraintConfig>& ptr) {
		return ptr.get() == pConstraintConfig;
		});

	if (it != configs.begin() && it != configs.end()) {
		// Find the index of the current element
		std::size_t currentIndex = std::distance(configs.begin(), it);
		// Calculate the index of the previous element
		return currentIndex;
	}

	return -1;
}

bool UTIL_ShiftUpConstraintIndex(CClientPhysicObjectConfig* pPhysicObjectConfig, int configId)
{
	auto& configs = pPhysicObjectConfig->ConstraintConfigs;

	auto it = std::find_if(configs.begin(), configs.end(), [configId](const std::shared_ptr<CClientConstraintConfig>& ptr) {
		return ptr->configId == configId;
		});

	if (it != configs.begin() && it != configs.end()) {
		auto currentIndex = std::distance(configs.begin(), it);
		auto prevIndex = currentIndex - 1;

		std::iter_swap(configs.begin() + currentIndex, configs.begin() + prevIndex);
		return true;
	}

	return false;
}

bool UTIL_ShiftDownConstraintIndex(CClientPhysicObjectConfig* pPhysicObjectConfig, int configId)
{
	auto& configs = pPhysicObjectConfig->ConstraintConfigs;

	auto it = std::find_if(configs.begin(), configs.end(), [configId](const std::shared_ptr<CClientConstraintConfig>& ptr) {
		return ptr->configId == configId;
		});

	if (it != configs.end() - 1 && it != configs.end()) {
		auto currentIndex = std::distance(configs.begin(), it);
		auto nextIndex = currentIndex + 1;

		std::iter_swap(configs.begin() + currentIndex, configs.begin() + nextIndex);
		return true;
	}

	return false;
}

bool UTIL_ShiftUpConstraintIndex(CClientPhysicObjectConfig* pPhysicObjectConfig, CClientConstraintConfig* pConstraintConfig)
{
	auto& configs = pPhysicObjectConfig->ConstraintConfigs;

	auto it = std::find_if(configs.begin(), configs.end(), [pConstraintConfig](const std::shared_ptr<CClientConstraintConfig>& ptr) {
		return ptr.get() == pConstraintConfig;
		});

	if (it != configs.begin() && it != configs.end()) {
		auto currentIndex = std::distance(configs.begin(), it);
		auto prevIndex = currentIndex - 1;

		std::iter_swap(configs.begin() + currentIndex, configs.begin() + prevIndex);
		return true;
	}

	return false;
}

bool UTIL_ShiftDownConstraintIndex(CClientPhysicObjectConfig* pPhysicObjectConfig, CClientConstraintConfig* pConstraintConfig)
{
	auto& configs = pPhysicObjectConfig->ConstraintConfigs;

	auto it = std::find_if(configs.begin(), configs.end(), [pConstraintConfig](const std::shared_ptr<CClientConstraintConfig>& ptr) {
		return ptr.get() == pConstraintConfig;
		});

	if (it != configs.end() - 1 && it != configs.end()) {
		auto currentIndex = std::distance(configs.begin(), it);
		auto nextIndex = currentIndex + 1;

		std::iter_swap(configs.begin() + currentIndex, configs.begin() + nextIndex);
		return true;
	}

	return false;
}

//PhysicBehavior stuffs

int UTIL_GetPhysicBehaviorIndex(const CClientPhysicObjectConfig* pPhysicObjectConfig, int configId)
{
	const auto& configs = pPhysicObjectConfig->PhysicBehaviorConfigs;

	auto it = std::find_if(configs.begin(), configs.end(), [configId](const std::shared_ptr<CClientPhysicBehaviorConfig>& ptr) {
		return ptr->configId == configId;
		});

	if (it != configs.end()) {
		return std::distance(configs.begin(), it);
	}

	return -1; // Return -1 if not found
}

int UTIL_GetPhysicBehaviorIndex(const CClientPhysicObjectConfig* pPhysicObjectConfig, const CClientPhysicBehaviorConfig* pPhysicBehaviorConfig)
{
	auto& configs = pPhysicObjectConfig->PhysicBehaviorConfigs;

	auto it = std::find_if(configs.begin(), configs.end(), [pPhysicBehaviorConfig](const std::shared_ptr<CClientPhysicBehaviorConfig>& ptr) {
		return ptr.get() == pPhysicBehaviorConfig;
		});

	if (it != configs.begin() && it != configs.end()) {
		// Find the index of the current element
		std::size_t currentIndex = std::distance(configs.begin(), it);
		// Calculate the index of the previous element
		return currentIndex;
	}

	return -1;
}

bool UTIL_ShiftUpPhysicBehaviorIndex(CClientPhysicObjectConfig* pPhysicObjectConfig, int configId)
{
	auto& configs = pPhysicObjectConfig->PhysicBehaviorConfigs;

	auto it = std::find_if(configs.begin(), configs.end(), [configId](const std::shared_ptr<CClientPhysicBehaviorConfig>& ptr) {
		return ptr->configId == configId;
		});

	if (it != configs.begin() && it != configs.end()) {
		auto currentIndex = std::distance(configs.begin(), it);
		auto prevIndex = currentIndex - 1;

		std::iter_swap(configs.begin() + currentIndex, configs.begin() + prevIndex);
		return true;
	}

	return false;
}

bool UTIL_ShiftDownPhysicBehaviorIndex(CClientPhysicObjectConfig* pPhysicObjectConfig, int configId)
{
	auto& configs = pPhysicObjectConfig->PhysicBehaviorConfigs;

	auto it = std::find_if(configs.begin(), configs.end(), [configId](const std::shared_ptr<CClientPhysicBehaviorConfig>& ptr) {
		return ptr->configId == configId;
		});

	if (it != configs.end() - 1 && it != configs.end()) {
		auto currentIndex = std::distance(configs.begin(), it);
		auto nextIndex = currentIndex + 1;

		std::iter_swap(configs.begin() + currentIndex, configs.begin() + nextIndex);
		return true;
	}

	return false;
}

bool UTIL_ShiftUpPhysicBehaviorIndex(CClientPhysicObjectConfig* pPhysicObjectConfig, CClientPhysicBehaviorConfig* pPhysicBehaviorConfig)
{
	auto& configs = pPhysicObjectConfig->PhysicBehaviorConfigs;

	auto it = std::find_if(configs.begin(), configs.end(), [pPhysicBehaviorConfig](const std::shared_ptr<CClientPhysicBehaviorConfig>& ptr) {
		return ptr.get() == pPhysicBehaviorConfig;
		});

	if (it != configs.begin() && it != configs.end()) {
		auto currentIndex = std::distance(configs.begin(), it);
		auto prevIndex = currentIndex - 1;

		std::iter_swap(configs.begin() + currentIndex, configs.begin() + prevIndex);
		return true;
	}

	return false;
}

bool UTIL_ShiftDownPhysicBehaviorIndex(CClientPhysicObjectConfig* pPhysicObjectConfig, CClientPhysicBehaviorConfig* pPhysicBehaviorConfig)
{
	auto& configs = pPhysicObjectConfig->PhysicBehaviorConfigs;

	auto it = std::find_if(configs.begin(), configs.end(), [pPhysicBehaviorConfig](const std::shared_ptr<CClientPhysicBehaviorConfig>& ptr) {
		return ptr.get() == pPhysicBehaviorConfig;
		});

	if (it != configs.end() - 1 && it != configs.end()) {
		auto currentIndex = std::distance(configs.begin(), it);
		auto nextIndex = currentIndex + 1;

		std::iter_swap(configs.begin() + currentIndex, configs.begin() + nextIndex);
		return true;
	}

	return false;
}

//AnimControl config order related

int UTIL_GetAnimControlIndex(const CClientRagdollObjectConfig* pRagdollObjectConfig, int configId)
{
	const auto& configs = pRagdollObjectConfig->AnimControlConfigs;

	auto it = std::find_if(configs.begin(), configs.end(), [configId](const std::shared_ptr<CClientAnimControlConfig>& ptr) {
		return ptr->configId == configId;
		});

	if (it != configs.end()) {
		return std::distance(configs.begin(), it);
	}

	return -1; // Return -1 if not found
}

int UTIL_GetAnimControlIndex(const CClientRagdollObjectConfig* pRagdollObjectConfig, const CClientAnimControlConfig* pAnimControlConfig)
{
	auto& configs = pRagdollObjectConfig->AnimControlConfigs;

	auto it = std::find_if(configs.begin(), configs.end(), [pAnimControlConfig](const std::shared_ptr<CClientAnimControlConfig>& ptr) {
		return ptr.get() == pAnimControlConfig;
		});

	if (it != configs.begin() && it != configs.end()) {
		// Find the index of the current element
		std::size_t currentIndex = std::distance(configs.begin(), it);
		// Calculate the index of the previous element
		return currentIndex;
	}

	return -1;
}

bool UTIL_ShiftUpAnimControlIndex(CClientRagdollObjectConfig* pRagdollObjectConfig, int configId)
{
	auto& configs = pRagdollObjectConfig->AnimControlConfigs;

	auto it = std::find_if(configs.begin(), configs.end(), [configId](const std::shared_ptr<CClientAnimControlConfig>& ptr) {
		return ptr->configId == configId;
		});

	if (it != configs.begin() && it != configs.end()) {
		auto currentIndex = std::distance(configs.begin(), it);
		auto prevIndex = currentIndex - 1;

		std::iter_swap(configs.begin() + currentIndex, configs.begin() + prevIndex);
		return true;
	}

	return false;
}

bool UTIL_ShiftDownAnimControlIndex(CClientRagdollObjectConfig* pRagdollObjectConfig, int configId)
{
	auto& configs = pRagdollObjectConfig->AnimControlConfigs;

	auto it = std::find_if(configs.begin(), configs.end(), [configId](const std::shared_ptr<CClientAnimControlConfig>& ptr) {
		return ptr->configId == configId;
		});

	if (it != configs.end() - 1 && it != configs.end()) {
		auto currentIndex = std::distance(configs.begin(), it);
		auto nextIndex = currentIndex + 1;

		std::iter_swap(configs.begin() + currentIndex, configs.begin() + nextIndex);
		return true;
	}

	return false;
}

bool UTIL_ShiftUpAnimControlIndex(CClientRagdollObjectConfig* pRagdollObjectConfig, CClientAnimControlConfig* pAnimControlConfig)
{
	auto& configs = pRagdollObjectConfig->AnimControlConfigs;

	auto it = std::find_if(configs.begin(), configs.end(), [pAnimControlConfig](const std::shared_ptr<CClientAnimControlConfig>& ptr) {
		return ptr.get() == pAnimControlConfig;
	});

	if (it != configs.begin() && it != configs.end()) {
		auto currentIndex = std::distance(configs.begin(), it);
		auto prevIndex = currentIndex - 1;

		std::iter_swap(configs.begin() + currentIndex, configs.begin() + prevIndex);
		return true;
	}

	return false;
}

bool UTIL_ShiftDownAnimControlIndex(CClientRagdollObjectConfig* pRagdollObjectConfig, CClientAnimControlConfig* pAnimControlConfig)
{
	auto& configs = pRagdollObjectConfig->AnimControlConfigs;

	auto it = std::find_if(configs.begin(), configs.end(), [pAnimControlConfig](const std::shared_ptr<CClientAnimControlConfig>& ptr) {
		return ptr.get() == pAnimControlConfig;
	});

	if (it != configs.end() - 1 && it != configs.end()) {
		auto currentIndex = std::distance(configs.begin(), it);
		auto nextIndex = currentIndex + 1;

		std::iter_swap(configs.begin() + currentIndex, configs.begin() + nextIndex);
		return true;
	}

	return false;
}

#undef min
#include <crc_32.h>

bool UTIL_GetCrc32ForBoneChunk(model_t *mod, std::string*output)
{
	if (mod->type != mod_studio)
		return false;

	auto studiohdr = (studiohdr_t*)IEngineStudio.Mod_Extradata(mod);

	if (!studiohdr)
		return false;

	if (!studiohdr->boneindex)
		return false;

	if (!studiohdr->numbones)
		return false;

	auto chunkBase = ((byte*)studiohdr + studiohdr->boneindex);
	size_t chunkSize = studiohdr->numbones * sizeof(mstudiobone_t);

	Chocobo1::CRC_32 hasher;

	hasher.addData(chunkBase, chunkSize);
	hasher.finalize();

	*output = hasher.toString();

	return true;
}

bool UTIL_GetCrc32ForModelFile(model_t* mod, std::string* output)
{
	if (mod->type == mod_studio)
	{
		auto studiohdr = (studiohdr_t*)IEngineStudio.Mod_Extradata(mod);

		if (!studiohdr)
			return false;

		size_t system_memory_length = 0;

		if (studiohdr->textureindex)
		{
			system_memory_length = (size_t)studiohdr->texturedataindex;
		}
		else
		{
			system_memory_length = (size_t)studiohdr->length;
		}

		auto chunkBase = ((byte*)studiohdr);
		size_t chunkSize = system_memory_length;

		Chocobo1::CRC_32 hasher;

		hasher.addData(chunkBase, chunkSize);
		hasher.finalize();

		*output = hasher.toString();

		return true;
	}
	else
	{
		//TODO: not impl...
	}
	return false;
}

bool UTIL_RebuildPhysicObjectWithClonedConfig(uint64_t physicObjectId, const CClientPhysicObjectConfig* pPhysicObjectConfig, int configId)
{
	auto pPhysicObject = ClientPhysicManager()->GetPhysicObjectEx(physicObjectId);

	if (pPhysicObject)
	{
		if (ClientPhysicManager()->RebuildPhysicObjectEx2(pPhysicObject, pPhysicObjectConfig))
		{
			int clonedPhysicComponentId = 0;

			pPhysicObject->EnumPhysicComponents([configId, &clonedPhysicComponentId](IPhysicComponent* pPhysicCompoent) {

				if (pPhysicCompoent->GetPhysicConfigId() == configId)
				{
					clonedPhysicComponentId = pPhysicCompoent->GetPhysicComponentId();
					return true;
				}

				return false;
			});

			if (clonedPhysicComponentId)
			{
				ClientPhysicManager()->SetSelectedPhysicComponentId(clonedPhysicComponentId);
				return true;
			}
		}
	}

	return false;
}
