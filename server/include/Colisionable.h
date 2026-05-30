#ifndef COLISIONABLE_H
#define COLISIONABLE_H

class Colisionable {
public:
    virtual ~Colisionable() = default;
    virtual bool colisionaCon(int x, int y, int ancho, int alto) const = 0;
    virtual int getX() const = 0;
    virtual int getY() const = 0;
    virtual int getAncho() const = 0;
    virtual int getAlto() const = 0;
};

#endif
