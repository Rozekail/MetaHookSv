#pragma once

#include <vgui_controls/Frame.h>
#include <vgui_controls/TextEntry.h>
#include <vgui_controls/CheckButton.h>
#include <vgui_controls/ComboBox.h>

#include "ClientPhysicConfig.h"

class CPhysicRigidBodyEditDialog : public vgui::Frame
{
public:
	DECLARE_CLASS_SIMPLE(CPhysicRigidBodyEditDialog, vgui::Frame);

	CPhysicRigidBodyEditDialog(vgui::Panel* parent, const char* name,
		uint64 physicObjectId,
		const std::shared_ptr<CClientPhysicObjectConfig>& pPhysicObjectConfig,
		const std::shared_ptr<CClientRigidBodyConfig>& pRigidBodyConfig);
	~CPhysicRigidBodyEditDialog();

	void Activate(void) override;

private:
	MESSAGE_FUNC_INT(OnRefreshCollisionShape, "RefreshCollisionShape", configId);
	MESSAGE_FUNC(OnResetData, "ResetData");
	void OnCommand(const char* command) override;

	void LoadAvailableShapesIntoControl(vgui::ComboBox* pComboBox);
	void LoadAvailableBonesIntoControl(vgui::ComboBox* pComboBox);
	void LoadShapeIntoControls(const CClientCollisionShapeConfigSharedPtr& collisionShape);
	void LoadBoneIntoControls(int boneindex);
	void LoadConfigIntoControls();
	void SaveConfigFromControls();
	void OnEditCollisionShape();
	int GetCurrentSelectedBoneIndex();

	typedef vgui::Frame BaseClass;

	vgui::TextEntry* m_pName{};
	vgui::TextEntry* m_pDebugDrawLevel{};
	vgui::ComboBox* m_pBone{};
	vgui::ComboBox* m_pShape{};
	vgui::TextEntry* m_pOriginX{};
	vgui::TextEntry* m_pOriginY{};
	vgui::TextEntry* m_pOriginZ{};
	vgui::TextEntry* m_pAnglesX{};
	vgui::TextEntry* m_pAnglesY{};
	vgui::TextEntry* m_pAnglesZ{};
	vgui::TextEntry* m_pMass{};
	vgui::TextEntry* m_pDensity{};
	vgui::TextEntry* m_pLinearFriction{};
	vgui::TextEntry* m_pRollingFriction{};
	vgui::TextEntry* m_pRestitution{};
	vgui::TextEntry* m_pCCDRadius{};
	vgui::TextEntry* m_pCCDThreshold{};
	vgui::TextEntry* m_pLinearSleepingThreshold{};
	vgui::TextEntry* m_pAngularSleepingThreshold{};
	vgui::CheckButton* m_pAlwaysDynamic{};
	vgui::CheckButton* m_pAlwaysKinematic{};
	vgui::CheckButton* m_pAlwaysStatic{};
	vgui::CheckButton* m_pNoCollisionToWorld{};
	vgui::CheckButton* m_pNoCollisionToStaticObject{};
	vgui::CheckButton* m_pNoCollisionToDynamicObject{};
	vgui::CheckButton* m_pNoCollisionToRagdollObject{};

	uint64 m_physicObjectId{};
	std::shared_ptr<CClientPhysicObjectConfig> m_pPhysicObjectConfig;
	std::shared_ptr<CClientRigidBodyConfig> m_pRigidBodyConfig;
};