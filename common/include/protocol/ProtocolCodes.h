#ifndef PROTOCOL_CODES_H
#define PROTOCOL_CODES_H

#include <cstdint>

/*
  Protocol Codes
  Tiene los distintos tipos de mensajes que se pueden enviar entre el
  cliente y el servidor.
*/

// Opcodes para comandos enviados por el cliente al servidor
enum class ClientCommandOpCode : uint8_t {
  RegisterPlayerCommand = 0x10,
  LoginPlayerCommand,
  MeditateCommand,
  PrivateMessageCommand,
  MoveCommand,
  ExitCommand,
  PlayerStopCommand,
  EquipCommand,
  UnequipCommand,
  DropItemCommand,
  TakeItemCommand,
  GlobalChatMessageCommand,
  CreateClanCommand,
  JoinClanCommand,
  AcceptClanRequestCommand,
  LeaveClanCommand,
  ReviewClanCommand,
  RejectClanRequestCommand,
  BanClanPlayerCommand,
  KickClanMemberCommand,
  AttackCommand,
  CheatCommand,
  ValidateLoginCommand,
};

// Opcodes para eventos enviados por el servidor a los clientes
enum class EventOpcode : uint8_t {
  ChatMessageEvent = 0x80,
  NPCDefeatedEvent,
  NPCRespawnedEvent,
  PlayerMovedEvent,
  RegisterPlayerEvent,
  PlayerListEvent,
  PlayerInfoEvent,
  PlayerAppearedEvent,
  PlayerStoppedEvent,
  PlayerRemovedEvent,
  NPCAppearedEvent,
  InventoryUpdateEvent,
  TextureInfoEvent,
  GroundItemAppearedEvent,
  GroundItemRemovedEvent,
  GroundItemsListEvent,
  PrivateMessageEvent,
  GlobalChatMessageEvent,
  NPCMovedEvent,
  NPCStoppedEvent,
  CityEntityAppearedEvent,
  CityEntityMovedEvent,
  CityEntityStoppedEvent,
  PlayerDieEvent,
  LoginResultEvent,
};

#endif // PROTOCOL_CODES_H
