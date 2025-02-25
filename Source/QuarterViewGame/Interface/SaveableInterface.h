#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
//#include "../Manager/SaveManager.h"
#include "SaveableInterface.generated.h"

class USaveGameDataBase;
enum class ESaveDataType : uint8;

UINTERFACE(MinimalAPI)
class USaveableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 세이브될 필요가 있는 요소를 위한 인터페이스
 * SaveManager에서 주로 사용됨
 */
class QUARTERVIEWGAME_API ISaveableInterface
{
	GENERATED_BODY()
	
public:
	virtual void LoadData(USaveGameDataBase& LoadData) = 0;
	virtual void SaveData(/*out*/USaveGameDataBase*& SaveData) = 0;
	virtual ESaveDataType SaveDataType() = 0;
};