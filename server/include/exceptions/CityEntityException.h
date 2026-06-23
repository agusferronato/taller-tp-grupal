#ifndef CITY_ENTITY_EXCEPTION_H
#define CITY_ENTITY_EXCEPTION_H

#include <stdexcept>

class ItemNotAvailable : public std::runtime_error {
public:
  ItemNotAvailable()
      : std::runtime_error("El item no esta disponible en esta tienda.") {}
};

class InsufficientGold : public std::runtime_error {
public:
  InsufficientGold() : std::runtime_error("No tienes suficiente oro.") {}
};

class PlayerDoesNotHaveTheItem : public std::runtime_error {
public:
  PlayerDoesNotHaveTheItem()
      : std::runtime_error("No tienes ese objeto en tu inventario.") {}
};

class ItemNotFoundInBank : public std::runtime_error {
public:
  ItemNotFoundInBank()
      : std::runtime_error(
            "Ese objeto no se encuentra en tu cuenta bancaria.") {}
};

class NoGoldAvailable : public std::runtime_error {
public:
  NoGoldAvailable()
      : std::runtime_error("No hay suficiente oro en tu cuenta bancaria.") {}
};

#endif
