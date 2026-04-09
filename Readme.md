
## 部署
最少需要填写以下内容

![](./Resources/MarkdownImg/PATypesToScan1.png)

![](./Resources/MarkdownImg/AssetMgr1.png)

![](./Resources/MarkdownImg/Mode_Inst.png)
这里可以先不填，因为最好按照项目的不同配置蓝图类GameInstance和Gamemode

DefaultGame.ini 需要添加:
```ini
[/Script/GameplayAbilities.AbilitySystemGlobals]
AbilitySystemGlobalsClassName=/Script/SkyraGame.SkyraAbilitySystemGlobals
GlobalGameplayCueManagerClass=/Script/SkyraGame.SkyraGameplayCueManager
PredictTargetGameplayEffects=false
bUseDebugTargetFromHud=true
ActivateFailIsDeadName=Ability.ActivateFail.IsDead
ActivateFailCooldownName=Ability.ActivateFail.Cooldown
ActivateFailCostName=Ability.ActivateFail.Cost
ActivateFailTagsBlockedName=Ability.ActivateFail.TagsBlocked
ActivateFailTagsMissingName=Ability.ActivateFail.TagsMissing
ActivateFailNetworkingName=Ability.ActivateFail.Networking
+GameplayCueNotifyPaths=/Game/GameplayCueNotifies
+GameplayCueNotifyPaths=/Game/GameplayCues

[/Script/GameFeatures.GameFeaturesSubsystemSettings]
GameFeaturesManagerClassName=/Script/SkyraGame.SkyraGameFeaturePolicy

[/Script/SkyraGame.SkyraAssetManager]
SkyraGameDataPath=/SkyraFramework/DefaultGameData.DefaultGameData ;默认值，根据需要更换配置
;DefaultPawnData=/Game/Characters/Heroes/EmptyPawnData/DefaultPawnData_EmptyPawn.DefaultPawnData_EmptyPawn  ;默认值，根据需要更换配置

[/Script/SkyraGame.SkyraUIManagerSubsystem]
DefaultUIPolicyClass=/SkyraFramework/UI/B_SkyraUIPolicy.B_SkyraUIPolicy_C  ;默认值，根据需要更换配置

[/Script/SkyraGame.SkyraUIMessaging]
;ConfirmationDialogClass=/Game/UI/Foundation/Dialogs/W_ConfirmationDefault.W_ConfirmationDefault_C ;确认窗口，如果没用到暂时不用配置
;ErrorDialogClass=/Game/UI/Foundation/Dialogs/W_ConfirmationError.W_ConfirmationError_C ;确认窗口，如果没用到暂时不用配置

[/Script/CommonLoadingScreen.CommonLoadingScreenSettings]
;LoadingScreenWidget=/Game/UI/Foundation/LoadingScreen/W_LoadingScreen_Host.W_LoadingScreen_Host_C ;默认值，根据需要更换配置
ForceTickLoadingScreenEvenInEditor=False

[/Script/CommonInput.CommonInputSettings]
InputData=/SkyraFramework/UI/B_CommonInputData.B_CommonInputData_C  ;默认值，可以不进行更换，根据需要更换配置
bEnableInputMethodThrashingProtection=True
InputMethodThrashingLimit=30
InputMethodThrashingWindowInSeconds=3.000000
InputMethodThrashingCooldownInSeconds=1.000000
bAllowOutOfFocusDeviceInput=True

[/Script/CommonUI.CommonUISettings]
DefaultThrobberMaterial=/SkyraFramework/UI/Foundation/Materials/M_UI_Throbber_Base.M_UI_Throbber_Base ;默认值，根据需要更换配置
DefaultRichTextDataClass=/SkyraFramework/UI/Foundation/RichTextData/CommonUIRichTextData.CommonUIRichTextData_C ;默认值，根据需要更换配置


[/Script/SkyraGame.SkyraAudioSettings] ;暂时没做迁入故先不配置
; DefaultControlBusMix=/Game/Audio/Modulation/ControlBusMixes/CBM_BaseMix.CBM_BaseMix
; UserSettingsControlBusMix=/Game/Audio/Modulation/ControlBusMixes/CBM_UserMix.CBM_UserMix
; OverallVolumeControlBus=/Game/Audio/Modulation/ControlBuses/CB_Main.CB_Main
; MusicVolumeControlBus=/Game/Audio/Modulation/ControlBuses/CB_Music.CB_Music
; SoundFXVolumeControlBus=/Game/Audio/Modulation/ControlBuses/CB_SFX.CB_SFX
; DialogueVolumeControlBus=/Game/Audio/Modulation/ControlBuses/CB_Dialogue.CB_Dialogue
; VoiceChatVolumeControlBus=/Game/Audio/Modulation/ControlBuses/CB_VoiceChat.CB_VoiceChat
; +HDRAudioSubmixEffectChain=(Submix="/Game/Audio/Submixes/MainSubmix.MainSubmix",SubmixEffectChain=("/Game/Audio/Effects/SubmixEffects/DYN_MainDynamics.DYN_MainDynamics"))
; +LDRAudioSubmixEffectChain=(Submix="/Game/Audio/Submixes/MainSubmix.MainSubmix",SubmixEffectChain=("/Game/Audio/DYN_LowMultibandDynamics.DYN_LowMultibandDynamics","/Game/Audio/Effects/SubmixEffects/DYN_LowDynamics.DYN_LowDynamics"))
; LoadingScreenControlBusMix=/Game/Audio/Modulation/ControlBusMixes/CBM_LoadingScreenMix.CBM_LoadingScreenMix

;以下为参考
; [/Script/Engine.AssetManagerSettings]
; -PrimaryAssetTypesToScan=(PrimaryAssetType="Map",AssetBaseClass=/Script/Engine.World,bHasBlueprintClasses=False,bIsEditorOnly=True,Directories=((Path="/Game/Maps")),SpecificAssets=,Rules=(Priority=-1,ChunkId=-1,bApplyRecursively=True,CookRule=Unknown))
; -PrimaryAssetTypesToScan=(PrimaryAssetType="PrimaryAssetLabel",AssetBaseClass=/Script/Engine.PrimaryAssetLabel,bHasBlueprintClasses=False,bIsEditorOnly=True,Directories=((Path="/Game")),SpecificAssets=,Rules=(Priority=-1,ChunkId=-1,bApplyRecursively=True,CookRule=Unknown))
; +PrimaryAssetTypesToScan=(PrimaryAssetType="Map",AssetBaseClass="/Script/Engine.World",bHasBlueprintClasses=False,bIsEditorOnly=True,Directories=((Path="/Game/Maps")),SpecificAssets=,Rules=(Priority=-1,ChunkId=-1,bApplyRecursively=True,CookRule=AlwaysCook))
; +PrimaryAssetTypesToScan=(PrimaryAssetType="PrimaryAssetLabel",AssetBaseClass="/Script/Engine.PrimaryAssetLabel",bHasBlueprintClasses=False,bIsEditorOnly=True,Directories=((Path="/Game")),SpecificAssets=,Rules=(Priority=-1,ChunkId=-1,bApplyRecursively=True,CookRule=Unknown))
; +PrimaryAssetTypesToScan=(PrimaryAssetType="SkyraGameData",AssetBaseClass="/Script/SkyraGame.SkyraGameData",bHasBlueprintClasses=False,bIsEditorOnly=False,Directories=,SpecificAssets=("/SkyraFramework/DefaultGameData.DefaultGameData"),Rules=(Priority=-1,ChunkId=-1,bApplyRecursively=True,CookRule=AlwaysCook))
; +PrimaryAssetTypesToScan=(PrimaryAssetType="GameFeatureData",AssetBaseClass="/Script/GameFeatures.GameFeatureData",bHasBlueprintClasses=False,bIsEditorOnly=False,Directories=((Path="/Game/Unused")),SpecificAssets=,Rules=(Priority=-1,ChunkId=-1,bApplyRecursively=True,CookRule=AlwaysCook))
; +PrimaryAssetTypesToScan=(PrimaryAssetType="SkyraExperienceDefinition",AssetBaseClass="/Script/SkyraGame.SkyraExperienceDefinition",bHasBlueprintClasses=True,bIsEditorOnly=False,Directories=((Path="/Game/System/Experiences"),(Path="/SkyraFramework/System/Experiences")),SpecificAssets=(None),Rules=(Priority=-1,ChunkId=-1,bApplyRecursively=True,CookRule=AlwaysCook))
; +PrimaryAssetTypesToScan=(PrimaryAssetType="SkyraUserFacingExperienceDefinition",AssetBaseClass="/Script/SkyraGame.SkyraUserFacingExperienceDefinition",bHasBlueprintClasses=False,bIsEditorOnly=False,Directories=((Path="/Game/UI/Temp"),(Path="/Game/System/Playlists")),SpecificAssets=,Rules=(Priority=-1,ChunkId=-1,bApplyRecursively=True,CookRule=AlwaysCook))
; +PrimaryAssetTypesToScan=(PrimaryAssetType="SkyraLobbyBackground",AssetBaseClass="/Script/SkyraGame.SkyraLobbyBackground",bHasBlueprintClasses=False,bIsEditorOnly=False,Directories=,SpecificAssets=,Rules=(Priority=-1,ChunkId=-1,bApplyRecursively=True,CookRule=AlwaysCook))
; +PrimaryAssetTypesToScan=(PrimaryAssetType="SkyraExperienceActionSet",AssetBaseClass="/Script/SkyraGame.SkyraExperienceActionSet",bHasBlueprintClasses=False,bIsEditorOnly=False,Directories=,SpecificAssets=,Rules=(Priority=-1,ChunkId=-1,bApplyRecursively=True,CookRule=AlwaysCook))

```

DefaultEngine.ini 需要添加:
```ini
[/Script/Engine.Engine] ;根据需要进行更换
DurationOfErrorsAndWarningsOnHUD=3.0
GameEngine=/Script/SkyraGame.SkyraGameEngine
UnrealEdEngine=/Script/SkyraEditor.SkyraEditorEngine
EditorEngine=/Script/SkyraEditor.SkyraEditorEngine
GameViewportClientClassName=/Script/SkyraGame.SkyraGameViewportClient
AssetManagerClassName=/Script/SkyraGame.SkyraAssetManager
WorldSettingsClassName=/Script/SkyraGame.SkyraWorldSettings
LocalPlayerClassName=/Script/SkyraGame.SkyraLocalPlayer
GameUserSettingsClassName=/Script/SkyraGame.SkyraSettingsLocal
NearClipPlane=3.000000

[/Script/EngineSettings.GameMapsSettings] ;根据需要进行更换
GlobalDefaultGameMode=/SkyraFramework/B_SkyraGameMode.B_SkyraGameMode_C
GameInstanceClass=/SkyraFramework/B_SkyraGameInstance.B_SkyraGameInstance_C

[/Script/Engine.Engine] ;根据需要进行更换
GameViewportClientClassName=/Script/SkyraGame.SkyraGameViewportClient
LocalPlayerClassName=/Script/SkyraGame.SkyraLocalPlayer
WorldSettingsClassName=/Script/SkyraGame.SkyraWorldSettings
GameUserSettingsClassName=/Script/SkyraGame.SkyraSettingsLocal
AssetManagerClassName=/Script/SkyraGame.SkyraAssetManager


[/Script/SignificanceManager.SignificanceManager]
SignificanceManagerClassName=/Script/SkyraGame.SkyraSignificanceManager
```


