# 언리얼 엔진 5 게임 클라이언트
## 🎮 프로젝트 소개
IOCP 서버와 연동하여 동작하는 언리얼 엔진 5 게임 입니다.
## 🗓 개발 기간
24.02.01 ~ 24.06.14
## 🖥 개발 환경 및 사용 언어
>Visual Studio 2022

>Unreal Engine 5.11

>C++
## 🕹 주요 클래스 소개
### ClientSocket (FRunnable 상속) - [자세히 보기](https://github.com/hyk3633/UntilDawn/wiki/ClientSocket-%ED%81%B4%EB%9E%98%EC%8A%A4-%EC%86%8C%EA%B0%9C)
- GameInstance가 멤버로 소유하고 싱글톤으로 구현
- 서버와 연결하고 데이터를 주고받는 클래스
- 플레이어 컨트롤러가 호출하는 각종 데이터 전송함수를 제공
- 서버로 부터 수신한 데이터를 메시지 큐에 채워 게임모드가 빼갈 수 있게 함
### GameModeLoginMap (AGameModeBase 상속) - [자세히 보기](https://github.com/hyk3633/UntilDawn/wiki/GameModeLoginMap-%ED%81%B4%EB%9E%98%EC%8A%A4-%EC%86%8C%EA%B0%9C)
- 로그인 맵에서 로그인 및 회원가입에 대한 처리를 담당하는 역할
- ClientSocket 객체의 메시지 큐로 부터 데이터를 빼와서 로그인, 회원가입과 같은 처리를 진행함 
### GameModeMainMap (AGameModeBase 상속) - [자세히 보기](https://github.com/hyk3633/UntilDawn/wiki/GameModeMainMap-%ED%81%B4%EB%9E%98%EC%8A%A4-%EC%86%8C%EA%B0%9C)
- 아이템, 좀비 캐릭터, 플레이어 캐릭터, 투사체와 같은 게임 내 모든 액터들을 총괄
- 메인 맵에서 동기화 등과 같은 액터들에 대한 처리를 담당하는 역할
- ClientSocket 객체의 메시지 큐로 부터 데이터를 빼와서 각종 패킷 타입에 따른 처리를 진행함
### PlayerCharacter (ACharacter 상속) - [자세히 보기](https://github.com/hyk3633/UntilDawn/wiki/PlayerCharacter-%ED%81%B4%EB%9E%98%EC%8A%A4-%EC%86%8C%EA%B0%9C)
- 플레이어가 조종하는 캐릭터
- Ability System Component의 아바타 액터를 담당
- PlayerController 여부를 통해 로컬 플레이어와 리모트 플레이어를 구분
### PlayerControllerMainMap (APlayerController 상속) - [자세히 보기](https://github.com/hyk3633/UntilDawn/wiki/PlayerControllerMainMap-%ED%81%B4%EB%9E%98%EC%8A%A4-%EC%86%8C%EA%B0%9C)
- 메인 맵에서 플레이어 캐릭터를 소유하는 컨트롤러
- 게임 내에서 그 게임을 하고 있는 단 하나의 로컬 플레이어에게만 배정되며 나머지 플레이어 캐릭터는 AIController가 배정됨
- Ability System Component의 오너 액터를 담당
- 각종 키 입력을 받아 플레이어 캐릭터로 전달하여 Gameplay Ability를 활성화하도록 함
- 입력에 따른 결과들을 ClientSocket 객체를 통해 서버로 전송하도록 함
- 아이템 장착, 무기 무장 등의 동작을 수행하고 연관된 객체들에 결과를 반영하도록 함
### InventoryComponent (UActorComponent 상속) - [자세히 보기](https://github.com/hyk3633/UntilDawn/wiki/InventoryComponent-%ED%81%B4%EB%9E%98%EC%8A%A4-%EC%86%8C%EA%B0%9C)
- PlayerControllerMainMap이 소유하는 액터 컴포넌트
- 플레이어가 획득한 아이템과 장착 무기를 관리하고 관련 정보 및 사용 함수 제공
### ItemManager (UActorComponent 상속) - [자세히 보기](https://github.com/hyk3633/UntilDawn/wiki/ItemManager-%ED%81%B4%EB%9E%98%EC%8A%A4-%EC%86%8C%EA%B0%9C)
- 게임 내의 아이템을 스폰하고 관리하는 역할
- 플레이어가 획득한 아이템, 장착한 아이템, 소모되어 파괴되는 아이템을 구분하여 관리
### ItemBase (AActor 상속) - [자세히 보기](https://github.com/hyk3633/UntilDawn/wiki/ItemBase-%ED%81%B4%EB%9E%98%EC%8A%A4-%EC%86%8C%EA%B0%9C)
- 아이템에서 외형을 담당하는 액터
- 플레이어가 아이템을 장착하는 경우 플레이어 캐릭터의 스켈레탈 메쉬에 부착됨
- 실질적인 데이터를 담당하는 ItemObject를 소유하며 ItemObject를 보관하는 그릇 역할을 한다고 볼 수 있음
### ItemObject (UObject 상속) - [자세히 보기](https://github.com/hyk3633/UntilDawn/wiki/ItemObject--%ED%81%B4%EB%9E%98%EC%8A%A4-%EC%86%8C%EA%B0%9C)
- 아이템에서 내부적인 데이터를 담고 있는 핵심, UObject를 상속함
- 플레이어가 아이템을 획득한 경우 ItemBase로 부터 꺼내어져 플레이어의 인벤토리에 저장되고 ItemBase는 비활성화 되고 오브젝트 풀링됨
- 아이템 ID, 아이템 타입, 수량, 타입에 따른 구체적인 정보 등과 같은 아이템의 모든 데이터를 가지고 있음
- 아이템 유형에 따라 소모 가능한 ItemConsumable, 무기나 장비 같은 영구 아이템인 ItemPermanent 파생 클래스로 나뉨
### ZombieCharacter (ACharacter 상속) - [자세히 보기](https://github.com/hyk3633/UntilDawn/wiki/ZombieCharacter-%ED%81%B4%EB%9E%98%EC%8A%A4-%EC%86%8C%EA%B0%9C)
- 게임에서 몬스터와 같은 역할
- 서버로부터 스테이트를 전달받아 움직임
- ChaseState 일 때는 다음에 이동할 위치를 전달받아 현재 위치와 다음 위치를 보간하며 이동함
