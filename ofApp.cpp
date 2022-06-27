#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(30);
	ofSetWindowTitle("openframeworks");

	ofBackground(0);
	ofSetCircleResolution(60);
	ofEnableBlendMode(ofBlendMode::OF_BLENDMODE_ADD);

	this->mesh.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);
	this->base_seed = glm::vec2(ofRandom(1000), ofRandom(1000));
}

//--------------------------------------------------------------
void ofApp::update() {

	this->mesh.clear();

	int span = 20;
	int radius = 90;
	ofColor color;

	auto deg_start = ofMap(ofNoise(39, ofGetFrameNum() * 0.005), 0, 1, -360, 360);
	for (int deg = deg_start; deg < deg_start + 360; deg += 45) {

		for (int k = 0; k < 4; k++) {

			auto location = glm::vec2(radius * cos(deg * DEG_TO_RAD), radius * sin(deg * DEG_TO_RAD));
			auto deg_param = ofMap(ofNoise(location.x * 0.005, location.y * 0.005, (ofGetFrameNum() + 1) * 0.005), 0, 1, -60, 60);
			auto next = glm::vec2((radius - 10) * cos((deg + deg_param) * DEG_TO_RAD), (radius - 10) * sin((deg + deg_param) * DEG_TO_RAD));

			auto distance = location - next;
			distance *= 1;

			auto future = location + distance * 30;
			auto random_deg = ofRandom(360);
			future += glm::vec2(120 * cos(random_deg * DEG_TO_RAD), 120 * sin(random_deg * DEG_TO_RAD));
			auto future_distance = future - location;

			this->location_list.push_back(location);
			this->velocity_list.push_back(glm::normalize(future_distance) * glm::length(distance) * ofRandom(0.25, 0.6));
			color.setHsb(ofRandom(255), 200, 255);
			this->color_list.push_back(ofColor(color, 0));
		}
	}

	for (int i = this->location_list.size() - 1; i > -1; i--) {

		this->location_list[i] += this->velocity_list[i];
		this->velocity_list[i] *= 1.01;

		if (glm::distance(glm::vec2(), this->location_list[i]) > 720) {

			this->location_list.erase(this->location_list.begin() + i);
			this->velocity_list.erase(this->velocity_list.begin() + i);
			this->color_list.erase(this->color_list.begin() + i);
		}

		this->mesh.addVertex(glm::vec3(this->location_list[i], 0));
		this->mesh.addColor(this->color_list[i]);
	}

	for (int i = 0; i < this->mesh.getNumVertices(); i++) {

		for (int k = i + 1; k < this->mesh.getNumVertices(); k++) {

			auto distance = glm::distance(this->mesh.getVertex(i), this->mesh.getVertex(k));
			if (distance < span) {

				auto alpha = distance < span * 0.25 ? 255 : ofMap(distance, span * 0.25, span, 255, 0);
				alpha = 255;

				if (this->mesh.getColor(i).a < alpha) {

					this->mesh.setColor(i, ofColor(this->mesh.getColor(i), alpha));
				}

				if (this->mesh.getColor(k).a < alpha) {

					this->mesh.setColor(k, ofColor(this->mesh.getColor(k), alpha));
				}

				this->mesh.addIndex(i);
				this->mesh.addIndex(k);
			}
		}
	}

}

//--------------------------------------------------------------
void ofApp::draw() {

	ofTranslate(ofGetWidth() * 0.5, ofGetHeight() * 0.5);

	this->mesh.drawWireframe();

	ofFill();
	for (int i = 0; i < this->mesh.getNumVertices(); i++) {

		ofSetColor(this->mesh.getColor(i));
		ofDrawCircle(this->mesh.getVertex(i), 3);
	}
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}