#include "Soul_Like_ACT.h"
#include "Engine/DataTable.h"
#include "ActionState.generated.h"

// 
// UENUM(BlueprintType)
// enum class EActionStateType : uint8
// {
// 	Idle,
// 	Moving,
// 	Attacking,
// 	Parrying,
// };
// 
// UENUM(BlueprintType)
// enum class EActionStateProgress : uint8
// {
// 	PreActing,
// 	InActing,
// 	PostActing,
// };
// 
// USTRUCT()
// struct FActionState : public FTableRowBase
// {
// 	GENERATED_BODY()
// 		
// 	URPOPERTY()
// 	EActionStateType ActionStateType;
// 
// 	URPOPERTY()
// 	EActionStateProgress ActionStateProgress;
// 
// 	URPOPERTY()
// 	TArray<ActionStateType> AvailableStates_During_PreActing;
// 
// 	URPOPERTY()
// 	TArray<ActionStateType> AvailableStates_During_InActing;
// 
// 	URPOPERTY()
// 	TArray<ActionStateType> AvailableStates_During_PostActing;
// 
// 	UFUNCTION(BlueprintCallable)
// 	bool CanSwitchActionState(EActionStateType TargetStateType)
// 	{
// 		if (ActionStateProgress == EActionStateProgress::PreActing)
// 		{
// 			return AvailableStates_During_PreActing.Contains(TargetStateType);
// 		}
// 		else if (ActionStateProgress == EActionStateProgress::InActing)
// 		{
// 			return AvailableStates_During_InActing.Contains(TargetStateType);
// 		}
// 		else if (ActionStateProgress == EActionStateProgress::PostActing)
// 		{
// 			return AvailableStates_During_PostActing.Contains(TargetStateType);
// 		}
// 		else
// 			return false;
// 	}
// 
// 	UFUNCTION(BlueprintCallable)
// 	FActionState* ActionStateProgressing()
// 	{
// 		++ActionStateProgress;
// 		
// 		if((uint8)ActionStateProgress>= sizeof(EActionStateProgress))
// 
// 	}
// 
// 	UFUNCTION(BlueprintCallable)
// 	FActionState* SwitchActionState(EActionStateType TargetStateType, EActionStateProgress = EActionStateProgress::PreActing, bool bForceSwitch = false)
// 	{
// 		
// 	}
// 
// 	UFUNCTION(BlueprintCallable)
// 	static FActionState* Instance()
// 	{
// 		static FActionState 
// 	}
// };