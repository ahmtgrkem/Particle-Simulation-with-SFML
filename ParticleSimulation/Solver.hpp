#pragma once
#include "Particle.hpp"
#include "SpatialGrid.hpp"
#include <iostream>

class Solver {
public:
	Solver() = default;

	Particle& objeEkle(sf::Vector2f konum, float yaricap) {
		Particle yeniParcacik = Particle(konum, yaricap);
		return objeler.emplace_back(yeniParcacik);
	}
	
	std::vector<Particle>& objeleriGetir() {
		return objeler;
	}

	void guncelle() {
		zaman += zaman_adimi;
		float alt_zaman_adimi = zaman_adimi / alt_adimlar;
		for (int i = 0; i < alt_adimlar; i++) {
			grid.temizle();
			for (auto& obje : objeler) {
				grid.ekle(obje);
			}
			yercekimiUygula();
			objeleriGuncelle(alt_zaman_adimi);
			sinirlariUygula();
			carpismalariKontrolEt();
		}
	}

	void sinirAta(sf::Vector2f konum, float genislik) {
		sinir_merkezi = konum;
		sinir_genislik = genislik;
	}

	void objeHizAta(Particle& obje, sf::Vector2f hiz) {
		obje.hizAta(hiz, zaman_adimi);
	}

	float zamanGetir() const {
		return zaman;
	}

	float zamanAdimiGetir() const {
		return zaman_adimi / static_cast<float>(alt_adimlar);
	}

	void zamanAdimiAta(uint32_t oran) {
		zaman_adimi = 1.0f / static_cast<float>(oran);
	}

private:
	std::vector<Particle> objeler;
	SpatialGrid grid{ 20.0f, 840 / 20, 840 / 20 };
	sf::Vector2f yercekimi = { 0.0f, 1000.0f };


	float zaman_adimi = 0.0f;
	float zaman = 0.0f;
	int alt_adimlar = 8;

	sf::Vector2f sinir_merkezi = { 420.0f, 420.0f };
	float sinir_genislik = 840.0f;

	void yercekimiUygula() {
		for (Particle& obje : objeler) {
			obje.ivmelendir(yercekimi);
		}
	}

	void carpismalariKontrolEt() {
		for (auto& obje1 : objeler) {
			auto yakin_parcaciklar = grid.query(obje1.konum, obje1.yaricap * 2.0f);

			for (auto* obje2_ptr : yakin_parcaciklar) {
				if (&obje1 == obje2_ptr) continue;

				Particle& obje2 = *obje2_ptr;
				sf::Vector2f carpisma_ekseni = obje1.konum - obje2.konum;
				float mesafe = std::hypot(carpisma_ekseni.x, carpisma_ekseni.y) + 0.001f;
				float min_mesafe = obje1.yaricap + obje2.yaricap;

				if (mesafe < min_mesafe) {
					sf::Vector2f normal = carpisma_ekseni / mesafe;
					const float delta = min_mesafe - mesafe;
					obje1.konum += 0.5f * delta * normal;
					obje2.konum -= 0.5f * delta * normal;
				}
			}
		}
	}

	void objeleriGuncelle(float dt) {
		for (Particle& obje : objeler) {
			obje.guncelle(dt);
		}
	}

	void sinirlariUygula() {
		float window_size = sinir_genislik;
		for (Particle& obje : objeler) {

			const sf::Vector2f konum = obje.konum;
			sf::Vector2f gecici_konum = obje.konum;
			const float dampening = 0.85f;
			sf::Vector2f hiz = obje.hizGetir();
			sf::Vector2f dy = { hiz.x, -hiz.y * dampening };
			sf::Vector2f dx = { -hiz.x * dampening , hiz.y};
			float particleSize = obje.yaricap * 2;

			if (konum.x < particleSize || konum.x + particleSize > window_size) { 
				if (konum.x < particleSize) gecici_konum.x = particleSize;
				if (konum.x + particleSize > window_size) gecici_konum.x = window_size - particleSize;
				obje.konum = gecici_konum;
				obje.hizAta(dx, 1.0f);
			}
			if (konum.y < particleSize || konum.y + particleSize > window_size) { 
				if (konum.y < particleSize) gecici_konum.y = particleSize;
				if (konum.y + particleSize > window_size) gecici_konum.y = window_size - particleSize;
				obje.konum = gecici_konum;
				obje.hizAta(dy, 1.0f);
			}
		}
	}

};