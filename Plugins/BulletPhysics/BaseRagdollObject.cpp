#include "BaseRagdollObject.h"
#include "BasePhysicManager.h"

#include "PhysicUTIL.h"

#include "privatehook.h"

CBaseRagdollObject::CBaseRagdollObject(const CPhysicObjectCreationParameter& CreationParam)
{
	m_entindex = CreationParam.m_entindex;
	m_entity = CreationParam.m_entity;
	m_model = CreationParam.m_model;
	m_model_scaling = CreationParam.m_model_scaling;
	m_playerindex = CreationParam.m_playerindex;
	m_configId = CreationParam.m_pPhysicObjectConfig->configId;
	m_flags = CreationParam.m_pPhysicObjectConfig->flags;
	m_debugDrawLevel = CreationParam.m_pPhysicObjectConfig->debugDrawLevel;

	m_RigidBodyConfigs = CreationParam.m_pPhysicObjectConfig->RigidBodyConfigs;
	m_ConstraintConfigs = CreationParam.m_pPhysicObjectConfig->ConstraintConfigs;
	m_ActionConfigs = CreationParam.m_pPhysicObjectConfig->ActionConfigs;
}

CBaseRagdollObject::~CBaseRagdollObject()
{
	DispatchFreePhysicComponents(m_PhysicComponents);
}

int CBaseRagdollObject::GetEntityIndex() const
{
	return m_entindex;
}

cl_entity_t* CBaseRagdollObject::GetClientEntity() const
{
	return m_entity;
}

entity_state_t* CBaseRagdollObject::GetClientEntityState() const
{
	if (ClientEntityManager()->IsEntityDeadPlayer(m_entity) || ClientEntityManager()->IsEntityPlayer(m_entity))
	{
		if (m_playerindex >= 0)
			return R_GetPlayerState(m_playerindex);
	}

	return &m_entity->curstate;
}

model_t* CBaseRagdollObject::GetModel() const
{
	return m_model;
}

float CBaseRagdollObject::GetModelScaling() const
{
	return m_model_scaling;
}

uint64 CBaseRagdollObject::GetPhysicObjectId() const
{
	return PACK_PHYSIC_OBJECT_ID(m_entindex, EngineGetModelIndex(m_model));
}

int CBaseRagdollObject::GetPlayerIndex() const
{
	return m_playerindex;
}

int CBaseRagdollObject::GetObjectFlags() const
{
	return m_flags;
}

int CBaseRagdollObject::GetPhysicConfigId() const
{
	return m_configId;
}

bool CBaseRagdollObject::IsClientEntityNonSolid() const
{
	if (GetActivityType() > StudioAnimActivityType_Idle)
		return false;

	return GetClientEntityState()->solid <= SOLID_TRIGGER ? true : false;
}

bool CBaseRagdollObject::ShouldDrawOnDebugDraw(const CPhysicDebugDrawContext* ctx) const
{
	if (m_debugDrawLevel > 0 && ctx->m_ragdollObjectLevel > 0 && ctx->m_ragdollObjectLevel >= m_debugDrawLevel)
		return true;

	return false;
}

bool CBaseRagdollObject::EnumPhysicComponents(const fnEnumPhysicComponentCallback& callback)
{
	for (auto pPhysicComponent : m_PhysicComponents)
	{
		if (callback(pPhysicComponent))
			return true;
	}

	return false;
}

bool CBaseRagdollObject::Rebuild(const CClientPhysicObjectConfig* pPhysicObjectConfig)
{
	if (pPhysicObjectConfig->type != PhysicObjectType_RagdollObject)
	{
		gEngfuncs.Con_DPrintf("Rebuild: pPhysicObjectConfig->type mismatch!\n");
		return false;
	}

	auto pRagdollObjectConfig = (CClientRagdollObjectConfig*)pPhysicObjectConfig;

	CPhysicObjectCreationParameter CreationParam;

	CreationParam.m_entity = GetClientEntity();
	CreationParam.m_entstate = GetClientEntityState();
	CreationParam.m_entindex = GetEntityIndex();
	CreationParam.m_model = GetModel();

	if (CreationParam.m_model->type == mod_studio)
	{
		CreationParam.m_studiohdr = (studiohdr_t*)IEngineStudio.Mod_Extradata(CreationParam.m_model);
		CreationParam.m_model_scaling = ClientEntityManager()->GetEntityModelScaling(CreationParam.m_entity, CreationParam.m_model);
	}

	CreationParam.m_playerindex = GetPlayerIndex();

	CreationParam.m_pPhysicObjectConfig = pRagdollObjectConfig;

	CPhysicComponentFilters filters;

	ClientPhysicManager()->RemovePhysicComponentsFromWorld(this, filters);

	//Rebuild everything

	m_AnimControlConfigs = pRagdollObjectConfig->AnimControlConfigs;

	for (const auto& pAnimConfig : m_AnimControlConfigs)
	{
		if (pAnimConfig->activity == StudioAnimActivityType_Idle)
		{
			m_IdleAnimConfig = pAnimConfig;
			break;
		}
	}

	for (const auto& pAnimConfig : m_AnimControlConfigs)
	{
		if (pAnimConfig->activity == StudioAnimActivityType_Debug)
		{
			m_DebugAnimConfig = pAnimConfig;
			break;
		}
	}

	if (CreationParam.m_model->type == mod_studio)
	{
		if (m_IdleAnimConfig)
		{
			ClientPhysicManager()->SetupBonesForRagdollEx(CreationParam.m_entity, CreationParam.m_entstate, CreationParam.m_model, CreationParam.m_entindex, CreationParam.m_playerindex, m_IdleAnimConfig.get());
		}
		else
		{
			ClientPhysicManager()->SetupBonesForRagdoll(CreationParam.m_entity, CreationParam.m_entstate, CreationParam.m_model, CreationParam.m_entindex, CreationParam.m_playerindex);
		}
	}

	m_RigidBodyConfigs = pRagdollObjectConfig->RigidBodyConfigs;
	m_ConstraintConfigs = pRagdollObjectConfig->ConstraintConfigs;
	m_ActionConfigs = pRagdollObjectConfig->ActionConfigs;

	m_keyBones.clear();
	m_nonKeyBones.clear();

	SaveBoneRelativeTransform(CreationParam);

	DispatchRebuildPhysicComponents(
		m_PhysicComponents,
		CreationParam, 
		m_RigidBodyConfigs, 
		m_ConstraintConfigs, 
		m_ActionConfigs, 
		std::bind(&CBaseRagdollObject::CreateRigidBody, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
		std::bind(&CBaseRagdollObject::AddRigidBody, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
		std::bind(&CBaseRagdollObject::CreateConstraint, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
		std::bind(&CBaseRagdollObject::AddConstraint, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
		std::bind(&CBaseRagdollObject::CreateAction, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
		std::bind(&CBaseRagdollObject::AddAction, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)
	);

	SetupNonKeyBones(CreationParam);

	InitCameraControl(&pRagdollObjectConfig->ThirdPersonViewCameraControlConfig, m_ThirdPersonViewCameraControl);
	InitCameraControl(&pRagdollObjectConfig->FirstPersonViewCameraControlConfig, m_FirstPersonViewCameraControl);

	return true;
}

void CBaseRagdollObject::Update(CPhysicObjectUpdateContext* ObjectUpdateContext)
{
	auto playerState = GetClientEntityState();

	if (m_bDebugAnimEnabled && m_DebugAnimConfig)
	{
		playerState->sequence = m_DebugAnimConfig->sequence;
		playerState->gaitsequence = m_DebugAnimConfig->gaitsequence;
		playerState->frame = m_DebugAnimConfig->frame;
		playerState->framerate = 0;

#define COPY_BYTE_ENTSTATE(attr, to, i) if (m_DebugAnimConfig->attr[i] >= 0 && m_DebugAnimConfig->attr[i] <= 255) to->attr[i] = m_DebugAnimConfig->attr[i];
		COPY_BYTE_ENTSTATE(controller, playerState, 0);
		COPY_BYTE_ENTSTATE(controller, playerState, 1);
		COPY_BYTE_ENTSTATE(controller, playerState, 2);
		COPY_BYTE_ENTSTATE(controller, playerState, 3);
		COPY_BYTE_ENTSTATE(blending, playerState, 0);
		COPY_BYTE_ENTSTATE(blending, playerState, 1);
		COPY_BYTE_ENTSTATE(blending, playerState, 2);
		COPY_BYTE_ENTSTATE(blending, playerState, 3);
#undef COPY_BYTE_ENTSTATE
	}

	auto iOldActivityType = GetActivityType();

	auto iNewActivityType = StudioGetSequenceActivityType(m_model, playerState);

	if (iNewActivityType == 0)
	{
		iNewActivityType = GetOverrideActivityType(playerState);
	}

	if (m_playerindex == m_entindex)
	{
		if (iNewActivityType == 1)
		{
			ClientEntityManager()->SetPlayerDeathState(m_playerindex, playerState, m_model);
		}
		else
		{
			ClientEntityManager()->ClearPlayerDeathState(m_playerindex);
		}
	}

	if (m_iBarnacleIndex && iNewActivityType != 2)
	{
		ReleaseFromBarnacle();
	}

	if (m_iGargantuaIndex && iNewActivityType != 2)
	{
		ReleaseFromGargantua();
	}

	if (UpdateActivity(iOldActivityType, iNewActivityType, playerState))
	{
		ObjectUpdateContext->m_bActivityChanged = true;

		//Transformed from whatever to barnacle
		if (iNewActivityType == 2 && iOldActivityType != 2)
		{
			auto pBarnacleObject = ClientPhysicManager()->FindBarnacleObjectForPlayer(playerState);

			if (pBarnacleObject)
			{
				ApplyBarnacle(pBarnacleObject);
			}
			else
			{
				auto pGargantuaEntity = ClientPhysicManager()->FindGargantuaObjectForPlayer(playerState);

				if (pGargantuaEntity)
				{
					ApplyGargantua(pGargantuaEntity);
				}
			}
		}

		//Transformed from death or barnacle to idle state.
		else if (iOldActivityType > 0 && iNewActivityType == 0)
		{
			ObjectUpdateContext->m_bRigidbodyResetPoseRequired = true;
		}
	}

	if (!ClientEntityManager()->IsEntityInVisibleList(GetClientEntity()))
	{
		ObjectUpdateContext->m_bRigidbodyUpdatePoseRequired = true;
	}

	DispatchPhysicComponentsUpdate(m_PhysicComponents, ObjectUpdateContext);

	if (ObjectUpdateContext->m_bRigidbodyResetPoseRequired && !ObjectUpdateContext->m_bRigidbodyPoseChanged)
	{
		ResetPose(playerState);

		ObjectUpdateContext->m_bRigidbodyPoseChanged = true;
	}

	if (ObjectUpdateContext->m_bRigidbodyUpdatePoseRequired && !ObjectUpdateContext->m_bRigidbodyPoseUpdated)
	{
		UpdatePose(playerState);

		ObjectUpdateContext->m_bRigidbodyPoseUpdated = true;
	}
}

bool CBaseRagdollObject::GetGoldSrcOriginAngles(float* origin, float* angles)
{
	if (m_ThirdPersonViewCameraControl.m_physicComponentId)
	{
		auto pRigidBody = UTIL_GetPhysicComponentAsRigidBody(m_ThirdPersonViewCameraControl.m_physicComponentId);

		if (pRigidBody)
		{
			return pRigidBody->GetGoldSrcOriginAnglesWithLocalOffset(m_ThirdPersonViewCameraControl.m_origin, m_ThirdPersonViewCameraControl.m_angles, origin, angles);
		}
	}

	for (auto pPhysicComponent : m_PhysicComponents)
	{
		if (pPhysicComponent->IsRigidBody())
		{
			auto pRigidBody = (IPhysicRigidBody*)pPhysicComponent;

			return pRigidBody->GetGoldSrcOriginAngles(origin, angles);
		}
	}


	return false;
}

bool CBaseRagdollObject::CalcRefDef(struct ref_params_s* pparams, bool bIsThirdPerson, void(*callback)(struct ref_params_s* pparams))
{
	if (GetActivityType() != StudioAnimActivityType_Idle)
	{
		if (bIsThirdPerson)
		{
			return SyncThirdPersonView(pparams, callback);
		}
		else
		{
			if (g_bIsCounterStrike && GetEntityIndex() == gEngfuncs.GetLocalPlayer()->index)
			{
				if (g_iUser1 && !(*g_iUser1))
					return false;
			}

			return SyncFirstPersonView(pparams, callback);
		}
	}

	return false;
}

bool CBaseRagdollObject::SyncThirdPersonView(struct ref_params_s* pparams, void(*callback)(struct ref_params_s* pparams))
{
	if (m_ThirdPersonViewCameraControl.m_physicComponentId)
	{
		auto pRigidBody = UTIL_GetPhysicComponentAsRigidBody(m_ThirdPersonViewCameraControl.m_physicComponentId);

		if (pRigidBody)
		{
			vec3_t vecGoldSrcNewOrigin;

			pRigidBody->GetGoldSrcOriginAnglesWithLocalOffset(m_ThirdPersonViewCameraControl.m_origin, m_ThirdPersonViewCameraControl.m_angles, vecGoldSrcNewOrigin, nullptr);

			vec3_t vecSavedSimOrgigin;

			VectorCopy(pparams->simorg, vecSavedSimOrgigin);

			VectorCopy(vecGoldSrcNewOrigin, pparams->simorg);

			callback(pparams);

			VectorCopy(vecSavedSimOrgigin, pparams->simorg);

			return true;
		}
	}

	return false;
}

bool CBaseRagdollObject::SyncFirstPersonView(struct ref_params_s* pparams, void(*callback)(struct ref_params_s* pparams))
{
	if (m_FirstPersonViewCameraControl.m_physicComponentId)
	{
		auto pRigidBody = UTIL_GetPhysicComponentAsRigidBody(m_FirstPersonViewCameraControl.m_physicComponentId);

		if (pRigidBody)
		{
			vec3_t vecGoldSrcNewOrigin, vecGoldSrcNewAngles;

			if (pRigidBody->GetGoldSrcOriginAnglesWithLocalOffset(m_FirstPersonViewCameraControl.m_origin, m_FirstPersonViewCameraControl.m_angles, vecGoldSrcNewOrigin, vecGoldSrcNewAngles))
			{
				vec3_t vecSavedSimOrgigin;
				vec3_t vecSavedClientViewAngles;
				VectorCopy(pparams->simorg, vecSavedSimOrgigin);
				VectorCopy(pparams->cl_viewangles, vecSavedClientViewAngles);
				int iSavedHealth = pparams->health;

				pparams->viewheight[2] = 0;
				VectorCopy(vecGoldSrcNewOrigin, pparams->simorg);
				VectorCopy(vecGoldSrcNewAngles, pparams->cl_viewangles);
				pparams->health = 1;

				callback(pparams);

				pparams->health = iSavedHealth;
				VectorCopy(vecSavedSimOrgigin, pparams->simorg);
				VectorCopy(vecSavedClientViewAngles, pparams->cl_viewangles);

				return true;
			}
		}
	}
	return false;
}

void CBaseRagdollObject::UpdatePose(entity_state_t* curstate)
{
	//Force bones update
	ClientPhysicManager()->UpdateBonesForRagdoll(GetClientEntity(), curstate, GetModel(), GetEntityIndex(), GetPlayerIndex());
}

bool CBaseRagdollObject::SetupBones(studiohdr_t* studiohdr)
{
	if (GetActivityType() == StudioAnimActivityType_Idle)
		return false;

	for (auto pPhysicComponent : m_PhysicComponents)
	{
		if (pPhysicComponent->IsRigidBody())
		{
			auto pRigidBody = (IPhysicRigidBody*)pPhysicComponent;

			pRigidBody->SetupBones(studiohdr);
		}
	}

	return true;
}

bool CBaseRagdollObject::SetupJiggleBones(studiohdr_t* studiohdr)
{
	for (auto pPhysicComponent : m_PhysicComponents)
	{
		if (pPhysicComponent->IsRigidBody())
		{
			auto pRigidBody = (IPhysicRigidBody*)pPhysicComponent;

			pRigidBody->SetupJiggleBones(studiohdr);
		}
	}

	return true;
}

bool CBaseRagdollObject::ResetPose(entity_state_t* curstate)
{
	auto mod = GetModel();

	if (!mod)
		return false;

	auto studiohdr = (studiohdr_t*)IEngineStudio.Mod_Extradata(mod);

	if (!studiohdr)
		return false;

	if (GetModel()->type == mod_studio)
		ClientPhysicManager()->SetupBonesForRagdoll(GetClientEntity(), curstate, GetModel(), GetEntityIndex(), GetPlayerIndex());

	for (auto pPhysicComponent : m_PhysicComponents)
	{
		if (pPhysicComponent->IsRigidBody())
		{
			auto pRigidBody = (IPhysicRigidBody*)pPhysicComponent;

			pRigidBody->ResetPose(studiohdr, curstate);
		}
	}

	return true;
}

void CBaseRagdollObject::ApplyBarnacle(IPhysicObject* pBarnacleObject)
{
	if (m_iBarnacleIndex)
		return;

	m_iBarnacleIndex = pBarnacleObject->GetEntityIndex();

	CPhysicObjectCreationParameter CreationParam;

	CreationParam.m_entity = GetClientEntity();
	CreationParam.m_entstate = GetClientEntityState();
	CreationParam.m_entindex = GetEntityIndex();
	CreationParam.m_model = GetModel();

	if (GetModel()->type == mod_studio)
	{
		CreationParam.m_studiohdr = (studiohdr_t*)IEngineStudio.Mod_Extradata(GetModel());
		CreationParam.m_model_scaling = ClientEntityManager()->GetEntityModelScaling(GetClientEntity(), GetModel());
	}

	CreationParam.m_playerindex = GetPlayerIndex();
	CreationParam.m_allowNonNativeRigidBody = true;

	for (auto pPhysicComponent : m_PhysicComponents)
	{
		if (pPhysicComponent->IsRigidBody())
		{
			auto pRigidBody = (IPhysicRigidBody*)pPhysicComponent;

			vec3_t vecZero = { 0, 0, 0 };

			pRigidBody->SetLinearVelocity(vecZero);
			pRigidBody->SetAngularVelocity(vecZero);
		}
	}

	for (const auto& pConstraintConfig : m_ConstraintConfigs)
	{
		const auto pConstraintConfigPtr = pConstraintConfig.get();

		if (!(pConstraintConfigPtr->flags & PhysicConstraintFlag_Barnacle))
			continue;

		auto pConstraint = CreateConstraint(CreationParam, pConstraintConfigPtr, 0);

		if (pConstraint)
		{
			AddConstraint(CreationParam, pConstraintConfigPtr, pConstraint);
		}
	}

	for (const auto& pActionConfig : m_ActionConfigs)
	{
		const auto pActionConfigPtr = pActionConfig.get();

		if (!(pActionConfigPtr->flags & PhysicActionFlag_Barnacle))
			continue;

		auto pPhysicAction = CreateAction(CreationParam, pActionConfigPtr, 0);

		if (pPhysicAction)
		{
			AddAction(CreationParam, pActionConfigPtr, pPhysicAction);
		}
	}
}

void CBaseRagdollObject::ReleaseFromBarnacle()
{
	if (!m_iBarnacleIndex)
		return;

	m_iBarnacleIndex = 0;

	CPhysicComponentFilters filters;

	filters.m_ConstraintFilter.m_HasWithFlags = true;
	filters.m_ConstraintFilter.m_WithFlags = PhysicConstraintFlag_Barnacle;

	filters.m_PhysicActionFilter.m_HasWithFlags = true;
	filters.m_PhysicActionFilter.m_WithFlags = PhysicActionFlag_Barnacle;

	ClientPhysicManager()->RemovePhysicComponentsFromWorld(this, filters);

	FreePhysicComponentsWithFilters(filters);
}

void CBaseRagdollObject::ApplyGargantua(IPhysicObject* pGargantuaObject)
{
	//TODO
}

void CBaseRagdollObject::ReleaseFromGargantua()
{
	//TODO
}

StudioAnimActivityType CBaseRagdollObject::GetActivityType() const
{
	return m_iActivityType;
}

StudioAnimActivityType CBaseRagdollObject::GetOverrideActivityType(entity_state_t* entstate) const
{
	for (const auto& AnimControlConfig : m_AnimControlConfigs)
	{
		if (AnimControlConfig->gaitsequence == 0)
		{
			if (entstate->sequence == AnimControlConfig->sequence)
			{
				return AnimControlConfig->activity;
			}
		}
		else
		{
			if (entstate->sequence == AnimControlConfig->sequence &&
				entstate->gaitsequence == AnimControlConfig->gaitsequence)
			{
				return AnimControlConfig->activity;
			}
		}
	}

	return StudioAnimActivityType_Idle;
}

int CBaseRagdollObject::GetBarnacleIndex() const
{
	return m_iBarnacleIndex;
}

int CBaseRagdollObject::GetGargantuaIndex() const
{
	return m_iGargantuaIndex;
}

bool CBaseRagdollObject::IsDebugAnimEnabled() const
{
	return m_bDebugAnimEnabled;
}

void CBaseRagdollObject::SetDebugAnimEnabled(bool bEnabled)
{
	m_bDebugAnimEnabled = bEnabled;
}

void CBaseRagdollObject::AddPhysicComponentsToPhysicWorld(void* world, const CPhysicComponentFilters& filters)
{
	for (auto pPhysicComponent : m_PhysicComponents)
	{
		if (!pPhysicComponent->IsAddedToPhysicWorld(world) && CheckPhysicComponentFilters(pPhysicComponent, filters))
		{
			pPhysicComponent->AddToPhysicWorld(world);
		}
	}
}

void CBaseRagdollObject::RemovePhysicComponentsFromPhysicWorld(void* world, const CPhysicComponentFilters& filters)
{
	for (auto itor = m_PhysicComponents.rbegin(); itor != m_PhysicComponents.rend(); itor++)
	{
		auto pPhysicComponent = (*itor);

		if (pPhysicComponent->IsAddedToPhysicWorld(world) && CheckPhysicComponentFilters(pPhysicComponent, filters))
		{
			pPhysicComponent->RemoveFromPhysicWorld(world);
		}
	}
}

void CBaseRagdollObject::FreePhysicComponentsWithFilters(const CPhysicComponentFilters& filters)
{
	DispatchFreePhysicCompoentsWithFilters(m_PhysicComponents, filters);
}

void CBaseRagdollObject::TransferOwnership(int entindex)
{
	m_entindex = entindex;
	m_entity = ClientEntityManager()->GetEntityByIndex(entindex);

	for (auto pPhysicComponent : m_PhysicComponents)
	{
		pPhysicComponent->TransferOwnership(entindex);
	}
}

IPhysicComponent* CBaseRagdollObject::GetPhysicComponentByName(const std::string& name)
{
	return DispatchGetPhysicComponentByName(m_PhysicComponents, name);
}

IPhysicComponent* CBaseRagdollObject::GetPhysicComponentByComponentId(int id)
{
	return DispatchGetPhysicComponentByComponentId(m_PhysicComponents, id);
}

IPhysicRigidBody* CBaseRagdollObject::GetRigidBodyByName(const std::string& name)
{
	return DispatchGetRigidBodyByName(m_PhysicComponents, name);
}

IPhysicRigidBody* CBaseRagdollObject::GetRigidBodyByComponentId(int id)
{
	return DispatchGetRigidBodyByComponentId(m_PhysicComponents, id);
}

IPhysicConstraint* CBaseRagdollObject::GetConstraintByName(const std::string& name)
{
	return DispatchGetConstraintByName(m_PhysicComponents, name);
}

IPhysicConstraint* CBaseRagdollObject::GetConstraintByComponentId(int id)
{
	return DispatchGetConstraintByComponentId(m_PhysicComponents, id);
}

IPhysicAction* CBaseRagdollObject::GetPhysicActionByName(const std::string& name)
{
	return DispatchGetPhysicActionByName(m_PhysicComponents, name);
}

IPhysicAction* CBaseRagdollObject::GetPhysicActionByComponentId(int id)
{
	return DispatchGetPhysicActionByComponentId(m_PhysicComponents, id);
}

IPhysicRigidBody* CBaseRagdollObject::FindRigidBodyByName(const std::string& name, bool allowNonNativeRigidBody)
{
	if (allowNonNativeRigidBody)
	{
		if (name.starts_with("@barnacle.") && m_iBarnacleIndex)
		{
			auto findName = name.substr(sizeof("@barnacle.") - 1);

			auto pBarnacleObject = ClientPhysicManager()->GetPhysicObject(m_iBarnacleIndex);

			if (pBarnacleObject)
			{
				return pBarnacleObject->GetRigidBodyByName(findName);
			}

			return nullptr;
		}
		else if (name.starts_with("@gargantua.") && m_iGargantuaIndex)
		{
			auto findName = name.substr(sizeof("@gargantua.") - 1);

			auto pGargantuaObject = ClientPhysicManager()->GetPhysicObject(m_iGargantuaIndex);

			if (pGargantuaObject)
			{
				return pGargantuaObject->GetRigidBodyByName(findName);
			}

			return nullptr;
		}
	}

	return GetRigidBodyByName(name);
}

void CBaseRagdollObject::SetupNonKeyBones(const CPhysicObjectCreationParameter& CreationParam)
{
	if (CreationParam.m_studiohdr)
	{
		for (int i = 0; i < CreationParam.m_studiohdr->numbones; ++i)
		{
			if (std::find(m_keyBones.begin(), m_keyBones.end(), i) == m_keyBones.end())
				m_nonKeyBones.emplace_back(i);
		}
	}
}

void CBaseRagdollObject::InitCameraControl(const CClientCameraControlConfig* pCameraControlConfig, CPhysicCameraControl& CameraControl)
{
	CameraControl = (*pCameraControlConfig);

	auto pRigidBody = GetRigidBodyByName(pCameraControlConfig->rigidbody);

	if (pRigidBody)
	{
		CameraControl.m_physicComponentId = pRigidBody->GetPhysicComponentId();
	}
}

void CBaseRagdollObject::SaveBoneRelativeTransform(const CPhysicObjectCreationParameter& CreationParam)
{

}

void CBaseRagdollObject::AddRigidBody(const CPhysicObjectCreationParameter& CreationParam, CClientRigidBodyConfig* pRigidBodyConfig, IPhysicRigidBody* pRigidBody)
{
	if (!pRigidBody)
		return;

	DispatchAddPhysicComponent(m_PhysicComponents, pRigidBody);

	if (CreationParam.m_studiohdr &&
		pRigidBodyConfig->boneindex >= 0 &&
		pRigidBodyConfig->boneindex < CreationParam.m_studiohdr->numbones)
	{
		m_keyBones.emplace_back(pRigidBodyConfig->boneindex);
	}
}

void CBaseRagdollObject::AddConstraint(const CPhysicObjectCreationParameter& CreationParam, CClientConstraintConfig* pConstraintConfig, IPhysicConstraint* pConstraint)
{
	if (!pConstraint)
		return;

	DispatchAddPhysicComponent(m_PhysicComponents, pConstraint);
}

void CBaseRagdollObject::AddAction(const CPhysicObjectCreationParameter& CreationParam, CClientPhysicActionConfig* pPhysicActionConfig, IPhysicAction* pPhysicAction)
{
	if (!pPhysicAction)
		return;

	DispatchAddPhysicComponent(m_PhysicComponents, pPhysicAction);
}

bool CBaseRagdollObject::UpdateActivity(StudioAnimActivityType iOldActivityType, StudioAnimActivityType iNewActivityType, entity_state_t* curstate)
{
	if (iOldActivityType == iNewActivityType)
		return false;

	//Start death animation
	if (iOldActivityType == 0 && iNewActivityType > 0)
	{
		const auto& found = std::find_if(m_AnimControlConfigs.begin(), m_AnimControlConfigs.end(), [curstate](const std::shared_ptr<CClientAnimControlConfig>& pConfig) {

			if (pConfig->sequence == curstate->sequence)
				return true;

			return false;
			});

		if (found != m_AnimControlConfigs.end())
		{
			if (curstate->frame < (*found)->frame)
			{
				return false;
			}
		}
	}

	m_iActivityType = iNewActivityType;
	return true;
}
