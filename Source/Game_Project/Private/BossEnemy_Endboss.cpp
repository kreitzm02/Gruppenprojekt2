// Fill out your copyright notice in the Description page of Project Settings.


#include "BossEnemy_Endboss.h"

#include "BoundariesChecker.h"
#include "Enemy_BurnGroundProjectile.h"
#include "Enemy_GolemBoulder.h"
#include "Enemy_MageFireballProjectile.h"
#include "Enemy_MageProjectile.h"
#include "FSM_EnemyStateMachineComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/OverlapResult.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/GameplayStatics.h"

ABossEnemy_Endboss::ABossEnemy_Endboss()
{
	m_weaponMesh->DestroyComponent();
	m_stateMachine->DestroyComponent();

	m_projectileSpawnLeftWeapon = CreateDefaultSubobject<UArrowComponent>(TEXT("ProjectileSpawnPointLeftWeapon"));
	m_projectileSpawnRightWeapon = CreateDefaultSubobject<UArrowComponent>(TEXT("ProjectileSpawnPointRightWeapon"));
	
}

void ABossEnemy_Endboss::OnConstruction(const FTransform& Transform)
{
	FName boneName = FName("ms03_01_Reaper_weapon001");
	m_projectileSpawnLeftWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, boneName);

	boneName = FName("ms03_01_Reaper_weapon");
	m_projectileSpawnRightWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, boneName);
}


void ABossEnemy_Endboss::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->MaxWalkSpeed = m_maxWalkSpeed;
	GetCharacterMovement()->MaxAcceleration = m_accelerationSpeed;

	m_baseMaxMoveSpeed = GetCharacterMovement()->MaxWalkSpeed;
	m_baseAcceleration = GetCharacterMovement()->MaxAcceleration;

	m_nextSpecialAtPercent -= m_specialsAfterLostHPPercent;

	m_leftHandAtkAnimLength = m_leftHandAtkAnim->GetPlayLength();
	m_rightHandAtkAnimLength = m_rightHandAtkAnim->GetPlayLength();
	m_bothHandsAtkAnimLength = m_bothHandsAtkAnim->GetPlayLength();

	m_aiController = Cast<AAIController>(GetController());

	TArray<FOverlapResult> overlaps;
	FCollisionQueryParams queryParams;
	queryParams.AddIgnoredActor(this);

	FCollisionObjectQueryParams objectQueryParams;
	objectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_GameTraceChannel1);

	bool hasOverlap = GetWorld()->OverlapMultiByObjectType(
		overlaps,
		GetActorLocation(),
		FQuat::Identity,
		objectQueryParams,
		FCollisionShape::MakeSphere(m_playerChaseRadius),
		queryParams
	);
	for (FOverlapResult& overlap : overlaps)
	{
		m_player = Cast<ACharacter>(overlap.GetActor());
		break;
	}


	FTransform transform = FTransform(FRotator(0.0f, 0.0f, 0.0f), GetActorLocation(), FVector(1.0f, 1.0f, 1.0f));

	ABoundariesChecker* boundariesChecker = GetWorld()->SpawnActorDeferred<ABoundariesChecker>(m_boundariesCheckerBP, transform);

	boundariesChecker->SetOwnerEnemy(this);
	boundariesChecker->SetPlayer(m_player);

	UGameplayStatics::FinishSpawningActor(boundariesChecker, transform);

	m_maxMultiplierAtHPPercent = 1 - m_maxMultiplierAtHPPercent;

	if (m_maxMultiplierAtHPPercent <= 0.0f)
	{
		m_maxMultiplierAtHPPercent = 0.01f;
	}

	m_skeletalMesh->PlayAnimation(m_idleAnimation, true);
}


void ABossEnemy_Endboss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//FireProjectileWithCount(m_player->GetActorLocation() - GetActorLocation(), m_projectileSpawnLeftWeapon->GetComponentLocation(), m_multipleProjectileAtkCount);
	//FireRandomBurnGroundWithCount(m_projectileSpawnLeftWeapon->GetComponentLocation(), m_burnGroundProjectileCount);

	if (!m_cornersSet)
	{
		if (!m_boundariesChecked)
		{
			return;
		}
		SetCorners();
	}

	UE_LOG(LogTemp, Error, TEXT("Xmin: %f"), m_xSizeMin)
	UE_LOG(LogTemp, Error, TEXT("Xmax: %f"), m_xSizeMax)
	UE_LOG(LogTemp, Error, TEXT("Ymin: %f"), m_ySizeMin)
	UE_LOG(LogTemp, Error, TEXT("Ymax: %f"), m_ySizeMax)


	if (m_isDoingSpecialAtk && !m_isDead)
	{
		//special attack
		m_atkTimer += DeltaTime;
		m_specialAtkTimer += DeltaTime;

		if (m_needAtkInit)
		{
			m_atkTimer = 999.0f;
			m_needAtkInit = false;
		}

		if (m_doLeftHandAtk && m_atkTimer >= m_leftHandAtkAnimLength)
		{
			m_skeletalMesh->PlayAnimation(m_leftHandAtkAnim, false);

			SetActorRotation(GetDirToPlayerNormalized().Rotation());

			m_atkTimer = 0.0f;

			m_currentProjectileSpawnPoint = m_projectileSpawnLeftWeapon->GetComponentLocation();

			m_doLeftHandAtk = false;
			m_shotFired = false;
		}
		else if (!m_doLeftHandAtk && m_atkTimer >= m_rightHandAtkAnimLength)
		{
			m_skeletalMesh->PlayAnimation(m_rightHandAtkAnim, false);

			SetActorRotation(GetDirToPlayerNormalized().Rotation());

			m_atkTimer = 0.0f;

			m_currentProjectileSpawnPoint = m_projectileSpawnRightWeapon->GetComponentLocation();

			m_doLeftHandAtk = true;
			m_shotFired = false;
		}

		if (m_atkTimer >= m_shootAtAnimOffset && !m_shotFired)
		{
			FireProjectileWithCount(m_player->GetActorLocation() - GetActorLocation(), m_currentProjectileSpawnPoint, m_multipleProjectileAtkCount);
			m_shotFired = true;
		}


		if (m_specialAtkTimer >= m_specialAtkDuration)
		{
			m_atkTimer = 0.0f;
			m_specialAtkTimer = 0.0f;
			m_isDoingSpecialAtk = false;
			m_needAtkInit = true;
		}
	}
	else if (!m_movingToEdge && !m_isDead)
	{
		if (PlayerInRange())
		{
			m_atkTimer += DeltaTime;

			if (m_needAtkInit)
			{
				m_skeletalMesh->PlayAnimation(m_bothHandsAtkAnim, false);

				//m_currentProjectileSpawnPoint = m_projectileSpawnLeftWeapon->GetComponentLocation();

				SetActorRotation(GetDirToPlayerNormalized().Rotation());

				m_needAtkInit = false;
			}

			if (m_restartAttack && m_atkTimer >= m_bothHandsAtkAnimLength)
			{
				m_skeletalMesh->PlayAnimation(m_bothHandsAtkAnim, false);

				SetActorRotation(GetDirToPlayerNormalized().Rotation());

				m_atkTimer = 0.0f;

				//m_currentProjectileSpawnPoint = m_projectileSpawnLeftWeapon->GetComponentLocation();

				m_doLeftHandAtk = false;
				m_shotFired = false;
			}

			if (m_atkTimer >= m_shootAtAnimOffset && !m_shotFired)
			{
				FireFireball(m_player, GetActorLocation());
				m_shotFired = true;
			}
		}
		else
		{
			if (!m_shotFired || m_atkTimer <= m_bothHandsAtkAnimLength)
			{
				m_atkTimer += DeltaTime;
				if (m_atkTimer >= m_shootAtAnimOffset && !m_shotFired)
				{
					FireFireball(m_player, GetActorLocation());
					m_shotFired = true;
				}
			}
			else
			{
				if (!m_needAtkInit)
				{
					m_needAtkInit = true;
				}
				m_skeletalMesh->PlayAnimation(m_chaseAnimation, true);
				AddMovementInput(GetDirToPlayerNormalized());
				SetActorRotation(GetDirToPlayerNormalized().Rotation());
			}
			
		}
	}
	else if (!m_isDead)
	{
		CheckIfMovedToEdge();
	}
}




float ABossEnemy_Endboss::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	m_currentHealth = FMath::Clamp(m_currentHealth - DamageAmount, 0.0f, m_maxHealth);

	m_currentDoStuffMultiplier = 1 + (1 - m_currentHealth / m_maxHealth) / m_maxMultiplierAtHPPercent * (m_doStuffMaxMultiplier - 1);
	m_currentDoStuffMultiplier = FMath::Clamp(m_currentDoStuffMultiplier, 1.0f, m_doStuffMaxMultiplier);

	if (m_nextSpecialAtPercent >= m_currentHealth / m_maxHealth && !m_isDead)
	{
		m_nextSpecialAtPercent -= m_specialsAfterLostHPPercent;
		//init for special attack
		MoveToPlayerFurthestEdge();

		GetWorld()->GetTimerManager().ClearTimer(m_periodicBoulderTimer);
		GetWorld()->GetTimerManager().SetTimer(m_periodicBoulderTimer, this, &ABossEnemy_Endboss::FireBoulderAbovePlayer, 10.0f / m_currentDoStuffMultiplier, true);
	}

	if (m_startFireBurnGroundAtHpPercent >= m_currentHealth / m_maxHealth && !m_periodicBurnGroundStarted)
	{
		GetWorld()->GetTimerManager().SetTimer(m_periodicBurnGroundTimer, this, &ABossEnemy_Endboss::FireRandomBurnGround, m_periodicBurnGroundTime, true);
		m_periodicBurnGroundStarted = true;
	}

	if (DamageCauser)
	{
		FVector knockbackDirection = GetActorLocation() - DamageCauser->GetActorLocation();
		knockbackDirection.Z = 0;
		knockbackDirection.Normalize();
		HandleKnockback(knockbackDirection, 600.0f /*get knockback strengh from damage causer*/);
		//ACharacter* damagingUnit = Cast<ACharacter>(DamageCauser);
		//damagingUnit->GetKnockback();
	}

	UpdateHealthBar();

	if (m_currentHealth <= 0.0f)
	{
		GetWorld()->GetTimerManager().ClearTimer(m_periodicBoulderTimer);
		GetWorld()->GetTimerManager().ClearTimer(m_periodicBurnGroundTimer);
		m_isDead = true;
		OnDeath();
	}

	return DamageAmount;
}





FVector ABossEnemy_Endboss::RandomVectorInBoundaries()
{
	float x = FMath::RandRange(m_xSizeMin, m_xSizeMax);
	float y = FMath::RandRange(m_ySizeMin, m_ySizeMax);
	float z = GetActorLocation().Z - GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	return FVector(x, y, z);
}

FVector ABossEnemy_Endboss::GetDirToPlayerNormalized()
{
	return (m_player->GetActorLocation() - GetActorLocation()).GetSafeNormal();
}

void ABossEnemy_Endboss::SetCorners()
{
	m_topLeft = FVector(m_xSizeMax, m_ySizeMin, GetActorLocation().Z);
	m_topRight = FVector(m_xSizeMax, m_ySizeMax, GetActorLocation().Z);
	m_botLeft = FVector(m_xSizeMin, m_ySizeMin, GetActorLocation().Z);
	m_botRight = FVector(m_xSizeMin, m_ySizeMax, GetActorLocation().Z);
	m_cornersSet = true;
}

void ABossEnemy_Endboss::ResetMoveSpeed()
{
	GetCharacterMovement()->MaxWalkSpeed = m_baseMaxMoveSpeed;
	GetCharacterMovement()->MaxAcceleration = m_baseAcceleration;
}


void ABossEnemy_Endboss::MoveToPlayerFurthestEdge()
{
	FVector playerLoc = m_player->GetActorLocation();

	float distance = FVector::Dist(playerLoc, m_topLeft);
	FVector moveLoc = m_topLeft;
	float distance2 = FVector::Dist(playerLoc, m_topRight);

	if (distance2 > distance)
	{
		distance = distance2;
		moveLoc = m_topRight;
	}
	distance2 = FVector::Dist(playerLoc, m_botLeft);
	if (distance2 > distance)
	{
		distance = distance2;
		moveLoc = m_botLeft;
	}
	distance2 = FVector::Dist(playerLoc, m_botRight);
	if (distance2 > distance)
	{
		moveLoc = m_botRight;
	}
	m_skeletalMesh->PlayAnimation(m_chaseAnimation, true);

	GetCharacterMovement()->MaxWalkSpeed *= 2;
	GetCharacterMovement()->MaxAcceleration *= 2;

	m_aiController->MoveToLocation(moveLoc);
	m_movingToEdge = true;
}

void ABossEnemy_Endboss::CheckIfMovedToEdge()
{
	float distance = FVector::Dist(GetActorLocation(), m_aiController->GetImmediateMoveDestination());

	if (distance <= 100.0f)
	{
		m_multipleProjectileAtkCount += 2;
		ResetMoveSpeed();
		m_movingToEdge = false;
		m_isDoingSpecialAtk = true;
	}
}

bool ABossEnemy_Endboss::PlayerInRange()
{
	float distance = FVector::Dist(m_player->GetActorLocation(),GetActorLocation());
	if (distance <= m_attackRange)
	{
		return true;
	}
	return false;
}

void ABossEnemy_Endboss::FireProjectile(FVector a_targetDir, FVector a_spawnPoint)
{
	FTransform transform = FTransform(FRotator(0.0f, 0.0f, 0.0f), a_spawnPoint, FVector(1.0f, 1.0f, 1.0f));

	AEnemy_MageProjectile* projectile = GetWorld()->SpawnActorDeferred<AEnemy_MageProjectile>(m_projectileBP, transform);
	projectile->SetOwnerEnemy(this);
	projectile->SetProjectileSpeed(m_projectileSpeed);
	projectile->SetProjectileLifeSpan(m_projectileLifetime);
	projectile->SetTargetDirection(a_targetDir);
	UGameplayStatics::FinishSpawningActor(projectile, transform);
}

void ABossEnemy_Endboss::FireProjectileWithCount(FVector a_targetDir, FVector a_spawnPoint, int a_count)
{
	float rotation = m_multiProjectileSpaceDegree;

	for (int i = 0; i < a_count; i++)
	{
		if (i == 0)
		{
			FireProjectile(a_targetDir, a_spawnPoint);
		}
		else if (i % 2 == 1)
		{
			FireProjectile(FRotator(0.0f, rotation, 0.0f).RotateVector(a_targetDir), a_spawnPoint);
		}
		else if (i % 2 == 0)
		{
			FireProjectile(FRotator(0.0f, rotation * -1, 0.0f).RotateVector(a_targetDir), a_spawnPoint);
			rotation += m_multiProjectileSpaceDegree;
		}
	}
}

void ABossEnemy_Endboss::FireFireball(AActor* a_target, FVector a_spawnPoint)
{
	FTransform transform = FTransform(FRotator(0.0f, 0.0f, 0.0f), a_spawnPoint, FVector(1.0f, 1.0f, 1.0f));

	AEnemy_MageFireballProjectile* fireball = GetWorld()->SpawnActorDeferred<AEnemy_MageFireballProjectile>(m_fireballBP, transform);
	fireball->SetOwnerEnemy(this);
	fireball->SetProjectileSpeed(m_fireballSpeed);
	fireball->SetProjectileLifeSpan(m_fireballLifetime);
	fireball->SetTarget(a_target);
	UGameplayStatics::FinishSpawningActor(fireball, transform);
}

void ABossEnemy_Endboss::FireBoulderAbovePlayer()
{
	FVector target = m_player->GetActorLocation();
	target.Z += m_boulderHeightOffset;

	FTransform transform = FTransform(FRotator::ZeroRotator, target, FVector::OneVector);
	AEnemy_GolemBoulder* boulder = GetWorld()->SpawnActorDeferred<AEnemy_GolemBoulder>(m_boulderBP, transform);
	boulder->SetOwner(this);
	UGameplayStatics::FinishSpawningActor(boulder, transform);

	UE_LOG(LogTemp, Warning, TEXT("Shockwave fired"))
}

void ABossEnemy_Endboss::FireBurnGround(FVector a_targetPos, FVector a_spawnPoint)
{
	FTransform transform = FTransform(FRotator(0.0f, 0.0f, 0.0f), a_spawnPoint, FVector(1.0f, 1.0f, 1.0f));

	AEnemy_BurnGroundProjectile* burnGround = GetWorld()->SpawnActorDeferred<AEnemy_BurnGroundProjectile>(m_burnGroundBP, transform);
	burnGround->SetOwner(this);
	burnGround->SetTargetPos(a_targetPos);
	burnGround->SetFlightTime(m_burnGroundFlightTime);
	burnGround->SetLifetime(m_burnGroundLifetime * m_currentDoStuffMultiplier);
	UGameplayStatics::FinishSpawningActor(burnGround, transform);
}

void ABossEnemy_Endboss::FireRandomBurnGroundWithCount(FVector a_spawnPoint, int a_count)
{
	for (int i = 0; i < a_count; i++)
	{
		FireBurnGround(RandomVectorInBoundaries(),a_spawnPoint);
	}
}

void ABossEnemy_Endboss::FireRandomBurnGround()
{
	for (int i = 0; i < m_burnGroundProjectileCount; i++)
	{
		FireBurnGround(RandomVectorInBoundaries(), GetActorLocation());
	}
}