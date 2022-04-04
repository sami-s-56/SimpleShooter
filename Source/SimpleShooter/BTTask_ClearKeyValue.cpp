// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_ClearKeyValue.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_ClearKeyValue::UBTTask_ClearKeyValue()
{
	NodeName = TEXT("Clear Blackboard Key");
}

EBTNodeResult::Type UBTTask_ClearKeyValue::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	OwnerComp.GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey());

	return EBTNodeResult::Succeeded;
}
