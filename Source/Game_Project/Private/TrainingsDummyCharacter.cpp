// Fill out your copyright notice in the Description page of Project Settings.


#include "TrainingsDummyCharacter.h"

#include "FSM_EnemyStateMachineComponent.h"

ATrainingsDummyCharacter::ATrainingsDummyCharacter()
{
	m_stateMachine->DestroyComponent();
}


void ATrainingsDummyCharacter::PreInitializeComponents()
{
	Super::PreInitializeComponents();

	m_weaponHitbox->SetActive(false);
	m_weaponMesh->SetActive(false);
	m_characterHitbox->SetActive(false);
}


void ATrainingsDummyCharacter::BeginPlay()
{
	Super::BeginPlay();

	m_defaultPosition = GetActorLocation();

	m_skeletalMesh->PlayAnimation(m_animation, true);

}


void ATrainingsDummyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (m_inCombat)
	{
		m_passedCombatTime += DeltaTime;

		if (m_passedCombatTime >= m_outOfCombatResetTime)
		{
			m_inCombat = false;
			m_passedCombatTime = 0.0f;
			m_currentHealth = m_maxHealth;
			UpdateHealthBar();
			SetActorLocation(m_defaultPosition);
		}
	}
}

float ATrainingsDummyCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	m_currentHealth = FMath::Clamp(m_currentHealth - DamageAmount, 0.0f, m_maxHealth);

	if (!m_inCombat)
	{
		m_inCombat = true;
	}

	m_passedCombatTime = 0.0f;

	UpdateHealthBar();

	m_receivingActionSoundComp->Sound = m_hitSound;
	m_receivingActionSoundComp->Play(0.0f);

	if (m_currentHealth <= 0.0f)
	{
		OnDeath();
	}

	return DamageAmount;
}

void ATrainingsDummyCharacter::OnDeath()
{
	m_currentHealth = m_maxHealth;
	UpdateHealthBar();
}