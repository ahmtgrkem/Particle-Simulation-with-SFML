#pragma once
#include "Particle.hpp"
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
		float alt_zaman_adimi = zaman_adimi / alt_adimlar;
		for (int i = 0; i < alt_adimlar; i++) {
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

private:
	std::vector<Particle> objeler;
	sf::Vector2f yercekimi = { 0.0f, 1000.0f };
	float zaman_adimi = 1.0f / 60.0f;
	int alt_adimlar = 8;

	sf::Vector2f sinir_merkezi = { 420.0f, 420.0f };
	float sinir_genislik = 840.0f;

	void yercekimiUygula() {
		for (Particle& obje : objeler) {
			obje.ivmelendir(yercekimi);
		}
	}

	void carpismalariKontrolEt()
	{
		int obje_sayisi = objeler.size();
		for (int i = 0; i < obje_sayisi; i++)
		{
			Particle& obje1 = objeler[i];
			for (int j = 0; j < obje_sayisi; j++)
			{
				if (i == j) continue;

				Particle& obje2 = objeler[j];

				sf::Vector2f carpisma_ekseni = obje1.konum - obje2.konum;

				float mesafe = sqrt(carpisma_ekseni.x * carpisma_ekseni.x + carpisma_ekseni.y * carpisma_ekseni.y) + 0.001f;

				float min_mesafe = obje1.yaricap + obje2.yaricap;

				if (mesafe < min_mesafe)
				{
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

			if (konum.x < particleSize || konum.x + particleSize > window_size) { // Kenarlardan sekme
				if (konum.x < particleSize) gecici_konum.x = particleSize;
				if (konum.x + particleSize > window_size) gecici_konum.x = window_size - particleSize;
				obje.konum = gecici_konum;
				obje.hizAta(dx, 1.0f);
			}
			if (konum.y < particleSize || konum.y + particleSize > window_size) { // Üstten/alttan sekme
				if (konum.y < particleSize) gecici_konum.y = particleSize;
				if (konum.y + particleSize > window_size) gecici_konum.y = window_size - particleSize;
				obje.konum = gecici_konum;
				obje.hizAta(dy, 1.0f);
			}
		}
	}

};