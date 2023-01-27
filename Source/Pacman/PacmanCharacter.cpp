// Copyright Epic Games, Inc. All Rights Reserved.

#include "PacmanCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "PacmanAbilitySystemComponent.h"
#include "PacmanAttributeSet.h"
#include "PacmanGameplayAbility.h"
#include <GameplayEffectTypes.h>
#include "Components/BoxComponent.h"

//////////////////////////////////////////////////////////////////////////
// APacmanCharacter

APacmanCharacter::APacmanCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)


	//Create the Ability System Component for Pacman
	AbilitySystemComponent = CreateDefaultSubobject<UPacmanAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Full);

	//Create the Attributes for Pacman
	Attributes = CreateDefaultSubobject<UPacmanAttributeSet>("Attributes");

	//BoxColliders
	HitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("HitBox"));
	HitBox-> SetupAttachment(RootComponent);
	HitBox-> OnComponentBeginOverlap.AddDynamic(this, &APacmanCharacter::OnHitBoxOverlap);
}

UAbilitySystemComponent* APacmanCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

//Add the init attributes for Pacman
void APacmanCharacter::InitializeAttributes()
{
	if (HasAuthority())
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
			Attributes->GetHealthAttribute()).AddUObject(this, &APacmanCharacter::HealthChanged);

		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
			Attributes->GetSpeedAttribute()).AddUObject(this, &APacmanCharacter::SpeedChanged);

		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
			Attributes->GetPelletsAttribute()).AddUObject(this, &APacmanCharacter::PelletsChanged);

		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
			Attributes->GetMaxHealthAttribute()).AddUObject(this, &APacmanCharacter::MaxHealthChanged);
	}
	
	if(AbilitySystemComponent && DefaultAttributeEffect)
	{
		FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
		EffectContext.AddSourceObject(this);

		FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(DefaultAttributeEffect, 1, EffectContext);

		if(SpecHandle.IsValid())
		{
			FActiveGameplayEffectHandle GEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}
	}


}

void APacmanCharacter::GiveAbilities()
{
	if(HasAuthority() && AbilitySystemComponent)
	{
		for (TSubclassOf<UPacmanGameplayAbility>& StartupAbility : DefaultAbilities) 
		{
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(
				StartupAbility,
				1,
				static_cast<int32>(StartupAbility.GetDefaultObject()->AbilityInputID), 
				this
			));
		}
	}
}

void APacmanCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	InitializeAttributes();
	//The server grants the default abilities to Pacman
	GiveAbilities();
}

void APacmanCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	InitializeAttributes();

	//Inits the input for the local player in order to control pacman abilities
	if (AbilitySystemComponent && InputComponent) 
	{
		const FGameplayAbilityInputBinds Binds(
			"Confirm",
			"Cancel",
			"EPacmanAbilityInputID",
			static_cast<int32>(EPacmanAbilityInputID::Confirm),
			static_cast<int32>(EPacmanAbilityInputID::Cancel)		
		);

		AbilitySystemComponent->BindAbilityActivationToInputComponent(InputComponent, Binds);
	}
}

void APacmanCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void APacmanCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APacmanCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APacmanCharacter::Look);

	}


	//Inits the input for the local player in order to control pacman abilities
	if (AbilitySystemComponent && InputComponent)
	{
		const FGameplayAbilityInputBinds Binds(
			"Confirm",
			"Cancel",
			"EPacmanAbilityInputID",
			static_cast<int32>(EPacmanAbilityInputID::Confirm),
			static_cast<int32>(EPacmanAbilityInputID::Cancel)
		);

		AbilitySystemComponent->BindAbilityActivationToInputComponent(InputComponent, Binds);
	}
}


void APacmanCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void APacmanCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}


void APacmanCharacter::HealthChanged(const FOnAttributeChangeData& Data)
{
	CurrentHealth = Data.NewValue;
}

void APacmanCharacter::SpeedChanged(const FOnAttributeChangeData& Data)
{
	UCharacterMovementComponent* characterMovement = GetCharacterMovement();
	characterMovement->MaxWalkSpeed = Data.NewValue;
}

void APacmanCharacter::PelletsChanged(const FOnAttributeChangeData& Data)
{
	CurrentPellets = Data.NewValue;
}

void APacmanCharacter::MaxHealthChanged(const FOnAttributeChangeData& Data)
{
	CurrentMaxHealth = Data.NewValue;
}

void APacmanCharacter::OnHitBoxOverlap(UPrimitiveComponent* OverlappedComponent, 
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	FString otherName = OtherActor->GetName();
	UE_LOG(LogTemp, Warning, TEXT("We just collide with : %s"), *otherName);

}
