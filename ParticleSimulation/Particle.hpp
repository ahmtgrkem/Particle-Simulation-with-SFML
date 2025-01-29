#pragma once
#include <SFML/Graphics.hpp>

class Particle {
public:
	sf::Vector2f konum;
	sf::Vector2f onceki_konum;
	sf::Vector2f ivme;
	float yaricap = 10.0f;



	Particle() = default;
	Particle(sf::Vector2f konum_, float yaricap_) :
		konum{ konum_ },
		onceki_konum{ konum_ },
		ivme{ 10.0f, 10.0f },
		yaricap{ yaricap_ }
	{}

	void guncelle(float dt) {
		sf::Vector2f yer_degistirme = konum - onceki_konum;
		onceki_konum = konum;
		konum = konum + yer_degistirme + ivme * (dt * dt);
		ivme = {};
	}

	void ivmelendir(sf::Vector2f ivme_) {
		ivme += ivme_;
	}

	void hizAta(sf::Vector2f hiz_, float dt) {
		onceki_konum = konum - (hiz_ * dt);
	}

	void hizEkle(sf::Vector2f hiz_, float dt) {
		onceki_konum -= hiz_ * dt;
	}

	sf::Vector2f hizGetir() {
		return konum - onceki_konum;
	}
};