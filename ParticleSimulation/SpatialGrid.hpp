#pragma once
#include <vector>
#include <unordered_map>
#include <SFML/System/Vector2.hpp>
#include "Particle.hpp"

class SpatialGrid {
public:
    SpatialGrid(float hucreBoyutu, int genislik, int yukseklik) : hucreBoyutu(hucreBoyutu), genislik(genislik), yukseklik(yukseklik){}
    void temizle() {
        grid.clear();
    }
    void ekle(Particle& particle) {
        std::pair<int, int> hucre = hucreGetir(particle.konum);
        int cellHash = hash(hucre.first, hucre.second);
        grid[cellHash].push_back(&particle);
    }
    std::vector<Particle*> query(const sf::Vector2f& konum, float yaricap) {
        std::vector<Particle*> result;
        std::pair<int, int> hucre = hucreGetir(konum);
        int minX = static_cast<int>(std::floor((konum.x - yaricap) / hucreBoyutu));
        int maxX = static_cast<int>(std::floor((konum.x + yaricap) / hucreBoyutu));
        int minY = static_cast<int>(std::floor((konum.y - yaricap) / hucreBoyutu));
        int maxY = static_cast<int>(std::floor((konum.y + yaricap) / hucreBoyutu));

        for (int x = minX; x <= maxX; ++x) {
            for (int y = minY; y <= maxY; ++y) {
                int cellHash = hash(x, y);
                if (grid.find(cellHash) != grid.end()) {
                    for (auto* particle : grid[cellHash]) {
                        float dist = std::sqrt(std::pow(particle->konum.x - konum.x, 2) +
                            std::pow(particle->konum.y - konum.y, 2));
                        if (dist <= yaricap) {
                            result.push_back(particle);
                        }
                    }
                }
            }
        }
        return result;
    }

private:
    float hucreBoyutu;
    int genislik, yukseklik;
    std::unordered_map<int, std::vector<Particle*>> grid;

    int hash(int x, int y) const {
        return (x * 19157) ^ (y * 75619);
    }
    std::pair<int, int> hucreGetir(const sf::Vector2f& konum) const {
        int x = static_cast<int>(std::floor(konum.x / hucreBoyutu));
        int y = static_cast<int>(std::floor(konum.y / hucreBoyutu));
        return { x, y };
    }
};