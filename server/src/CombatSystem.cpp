#include "CombatSystem.h"
#include "Formulas.h"
#include <algorithm>

CombatSystem::CombatSystem(
    PlayerService &playerService, std::list<std::unique_ptr<NPC>> &npcs,
    const std::list<City> &cities, std::vector<Colisionable *> &colisionables,
    const std::list<std::unique_ptr<Biome>> &biomes,
    std::list<ServerEventDTO> &messagesToSend,
    SenderQueueMonitor &senderQueueMonitor, InventoryManager &inventoryManager,
    const ClanManager &clanManager,
    std::unordered_map<uint32_t, PlayerCheats> &cheatsByPlayer,
    const int gridSize, const int maxSize)
    : playerService(playerService), npcs(npcs), cities(cities),
      colisionables(colisionables), biomes(biomes),
      messagesToSend(messagesToSend), senderQueueMonitor(senderQueueMonitor),
      inventoryManager(inventoryManager), clanManager(clanManager),
      cheatsByPlayer(cheatsByPlayer), gridSize(gridSize), maxSize(maxSize) {}

void CombatSystem::attack(Character &attacker, int16_t x, int16_t y) {
  if (!attacker.assertAttackDistance(x, y)) {
    return;
  }

  NPC *targetNPC = findNPCByCoordinates(x, y);
  if (targetNPC != nullptr) {
    playerAttackNPC(attacker, *targetNPC);
    return;
  }

  Character *targetPlayer = findPlayerByCoordinates(x, y);
  if (targetPlayer != nullptr) {
    playerAttackPlayer(attacker, *targetPlayer);
    return;
  }
}

void CombatSystem::tryAttack(NPC &npc, Character &target) {
  if (!npc.collidesWith(target) || !npc.reachesAttackCounter()) {
    return;
  }
  if (target.isMeditating()) {
    target.stopMeditating();
    senderQueueMonitor.sendToClient(
        target.getId(), ChatMessageEventDTO{ChatMessageCategory::System,
                                            "Sistema", "Dejaste de meditar."});
  }

  if (target.tryParry()) {
    senderQueueMonitor.sendToClient(
        target.getId(),
        ChatMessageEventDTO{ChatMessageCategory::Combat, "Sistema",
                            npc.getName() +
                                " trato de atacarte pero lo esquivaste"});
    return;
  }

  target.takeDamage(npc.getDamage());

  if (target.getHp() <= 0) {
    killPlayer(target);
    return;
  }

  messagesToSend.push_back(AttackReceivedEventDTO{
      EntityType::Player, target.getId(), EffectType::None});
  messagesToSend.push_back(target.toPlayerInfoEvent());
}

bool CombatSystem::hasInfiniteHealth(uint32_t playerId) const {
  auto it = cheatsByPlayer.find(playerId);
  return it != cheatsByPlayer.end() && it->second.infiniteHealth;
}

void CombatSystem::playerAttackPlayer(Character &attacker, Character &target) {

  Weapon weapon =
      attacker.getEquippedWeapon(); // no deberia estar en Character.cpp?

  if (!weapon.isHealing() && !validAttack(attacker, target)) {
    return;
  }

  if (!consumeManaForAttack(attacker))
    return;

  if (weapon.isHealing()) {

    target.heal(weapon.healValue());
    messagesToSend.push_back(AttackReceivedEventDTO{
        EntityType::Player, target.getId(), weapon.effectType()});
    messagesToSend.push_back(attacker.toPlayerInfoEvent());
    messagesToSend.push_back(target.toPlayerInfoEvent());
    return;
  }

  if (target.isMeditating()) {
    target.stopMeditating();
    senderQueueMonitor.sendToClient(
        target.getId(), ChatMessageEventDTO{ChatMessageCategory::System,
                                            "Sistema", "Dejaste de meditar."});
  }

  uint32_t damage = calculateDamage(attacker);
  bool critical = (damage != attacker.getDamage());

  if (!critical && target.tryParry()) {
    senderQueueMonitor.sendToClient(
        attacker.getId(),
        ChatMessageEventDTO{ChatMessageCategory::Combat, "Sistema",
                            "Atacaste a " + target.getName() +
                                " pero el lo esquivo"});
    senderQueueMonitor.sendToClient(
        target.getId(),
        ChatMessageEventDTO{ChatMessageCategory::Combat, "Sistema",
                            attacker.getName() +
                                " trato de atacarte pero lo esquivaste"});
    return;
  }
  if (hasInfiniteHealth(target.getId())) {
    damage = 0;
  } else {
    damage = target.takeDamage(damage);
  }

  messagesToSend.push_back(AttackReceivedEventDTO{
      EntityType::Player, target.getId(), weapon.effectType()});

  uint32_t xp = Formulas::calcularExperiencia(damage, attacker.getLevel(),
                                              target.getLevel());
  attacker.gainExperience(xp);

  if (target.getHp() <= 0) {
    uint32_t gold = target.dropGoldOnDeath();
    attacker.addGold(gold);
    uint32_t xpDeath = Formulas::calcularExperienciaMuerte(
        target.getMaxHp(), attacker.getLevel(), target.getLevel(),
        (std::rand() % 100) / 100.0);
    attacker.gainExperience(xpDeath);
    killPlayer(target);
  } else {
    senderQueueMonitor.sendToClient(
        attacker.getId(),
        ChatMessageEventDTO{ChatMessageCategory::Combat, "Sistema",
                            "Atacaste a " + target.getName() +
                                " y le hiciste " + std::to_string(damage) +
                                " de daño!"});
    senderQueueMonitor.sendToClient(
        target.getId(),
        ChatMessageEventDTO{ChatMessageCategory::Combat, "Sistema",
                            "Recibiste un ataque de " + attacker.getName() +
                                " y te hicieron " + std::to_string(damage) +
                                " de daño!"});
  }
  messagesToSend.push_back(attacker.toPlayerInfoEvent());
  messagesToSend.push_back(target.toPlayerInfoEvent());
}
void CombatSystem::playerAttackNPC(Character &attacker, NPC &target) {

  Weapon weapon = attacker.getEquippedWeapon();
  if (weapon.isHealing())
    return; // No se puede curar NPCs

  if (!validAttackToNpc(attacker))
    return;

  if (!consumeManaForAttack(attacker))
    return;

  uint32_t damage = calculateDamage(attacker);

  bool critico = (damage != attacker.getDamage());

  if (!critico && target.tryParry()) {
    senderQueueMonitor.sendToClient(
        attacker.getId(),
        ChatMessageEventDTO{ChatMessageCategory::Combat, "Sistema",
                            "Atacaste a " + target.getName() +
                                " pero lo esquivo"});
    return;
  }

  target.takeDamage(damage);

  messagesToSend.push_back(AttackReceivedEventDTO{
      EntityType::Npc, target.getId(), weapon.effectType()});

  uint32_t xp = Formulas::calcularExperiencia(damage, attacker.getLevel(),
                                              target.getLevel());
  attacker.gainExperience(xp);

  if (target.getHP() <= 0) {

    ObjectDropped objectDropped = target.getDroppedObject();

    switch (objectDropped.type) {
    case ObjectDroppedType::Gold:
      attacker.addGold(objectDropped.value);
      break;

    case ObjectDroppedType::Item:
      inventoryManager.addGroundItem(static_cast<uint8_t>(objectDropped.value),
                                     target.getX(), target.getY());
      break;

    default:
      break;
    }

    uint32_t xpDeath = Formulas::calcularExperienciaMuerte(
        target.getMaxHp(), attacker.getLevel(), target.getLevel(),
        (std::rand() % 100) / 100.0);
    attacker.gainExperience(xpDeath);

    messagesToSend.push_back(NpcDefeatedEventDTO{target.getId()});

    colisionables.erase(
        std::remove(colisionables.begin(), colisionables.end(), &target),
        colisionables.end());

    auto npcIt =
        std::find_if(npcs.begin(), npcs.end(), [&target](const auto &npc) {
          return npc.get() == &target;
        });
    if (npcIt != npcs.end()) {
      uint32_t npcId = (*npcIt)->getId();
      for (auto &biome : biomes) {
        if (biome->hasNPC(npcId)) {
          biome->unregisterNPC(npcId);
          break;
        }
      }
      npcs.erase(npcIt);
    }

  } else {
    senderQueueMonitor.sendToClient(
        attacker.getId(),
        ChatMessageEventDTO{ChatMessageCategory::Combat, "Sistema",
                            "Atacaste a un " + target.getName() +
                                " y le hiciste " + std::to_string(damage) +
                                " de daño!"});
  }
  messagesToSend.push_back(attacker.toPlayerInfoEvent());
}

bool CombatSystem::validAttack(Character &attacker, Character &target) {

  // No puedes atacarte a ti mismo
  if (attacker.getId() == target.getId()) {
    return false;
  }

  if (attacker.isNewbie()) {
    senderQueueMonitor.sendToClient(
        attacker.getId(),
        ChatMessageEventDTO{
            ChatMessageCategory::Combat, "Sistema",
            "No podes atacar a otros jugadores siendo newbie."});
    return false;
  }

  if (target.isNewbie()) {
    senderQueueMonitor.sendToClient(
        attacker.getId(),
        ChatMessageEventDTO{ChatMessageCategory::Combat, "Sistema",
                            "No podes atacar a un jugador newbie."});
    return false;
  }

  if (clanManager.sameClan(attacker.getName(), target.getName())) {
    senderQueueMonitor.sendToClient(
        attacker.getId(),
        ChatMessageEventDTO{ChatMessageCategory::Combat, "Sistema",
                            "No podes atacar a un miembro de tu clan."});
    return false;
  }
  if (abs(static_cast<int>(attacker.getLevel()) -
          static_cast<int>(target.getLevel())) > 10) {
    senderQueueMonitor.sendToClient(
        attacker.getId(),
        ChatMessageEventDTO{ChatMessageCategory::Combat, "Sistema",
                            "No podes atacar a un jugador con tanta diferencia "
                            "de nivel."});
    return false;
  }
  for (const auto &city : cities) {
    if (city.contains(attacker.getX(), attacker.getY(), gridSize, maxSize) ||
        city.contains(target.getX(), target.getY(), gridSize, maxSize)) {
      senderQueueMonitor.sendToClient(
          attacker.getId(),
          ChatMessageEventDTO{
              ChatMessageCategory::Combat, "Sistema",
              "No podes atacar jugadores dentro de una ciudad."});
      return false;
    }
  }
  if (attacker.isDead() || target.isDead()) {
    senderQueueMonitor.sendToClient(
        attacker.getId(),
        ChatMessageEventDTO{ChatMessageCategory::Combat, "Sistema",
                            "No podes atacar o ser atacado estando muerto."});
    return false;
  }
  return true;
}

bool CombatSystem::validAttackToNpc(Character &attacker) {
  if (attacker.isDead())
    return false;
  for (const auto &city : cities) {
    if (city.contains(attacker.getX(), attacker.getY(), gridSize, maxSize)) {
      return false;
    }
  }
  return true;
}

Character *CombatSystem::findPlayerByCoordinates(int16_t x, int16_t y) {
  for (auto &[pid, player] : playerService.getPlayers()) {
    if (player->colisionaCon(x, y, 16, 16)) {
      return player.get();
    }
  }
  return nullptr;
}

NPC *CombatSystem::findNPCByCoordinates(int16_t x, int16_t y) {
  for (auto &npc : npcs) {
    if (npc->colisionaCon(x, y, 16, 16)) {
      return npc.get();
    }
  }
  return nullptr;
}

uint32_t CombatSystem::calculateDamage(Character &attacker) {
  uint32_t damage = attacker.getDamage();
  if (Formulas::calcularCritico(std::rand())) {
    return damage * 2;
  }
  return damage;
}

bool CombatSystem::consumeManaForAttack(Character &attacker) {
  // tal vez deberia estar en Charater.cpp
  Weapon weapon = attacker.getEquippedWeapon();
  if (weapon.manaCost() <= 0) {
    return true;
  }
  if (!attacker.useMana(weapon.manaCost())) {
    senderQueueMonitor.sendToClient(
        attacker.getId(),
        ChatMessageEventDTO{ChatMessageCategory::Combat, "Sistema",
                            "No tenes suficiente mana para atacar."});
    return false;
  }
  return true;
}

void CombatSystem::killPlayer(Character &dyingPlayer, bool dropExcessGold) {
  if (dyingPlayer.isMeditating()) {
    dyingPlayer.stopMeditating();
    senderQueueMonitor.sendToClient(
        dyingPlayer.getId(),
        ChatMessageEventDTO{ChatMessageCategory::System, "Sistema",
                            "Dejaste de meditar."});
  }

  if (dropExcessGold) {
    dyingPlayer.dropGoldOnDeath();
  }
  auto items = dyingPlayer.die();
  int16_t x = dyingPlayer.getX();
  int16_t y = dyingPlayer.getY();
  for (auto itemId : items) {
    inventoryManager.addGroundItem(itemId, x, y);
  }
  messagesToSend.push_back(PlayerDieEventDTO{dyingPlayer.getId()});
}
