#include <quadtree.h>
#include <avr/dtostrf.h>

//General
#define Serial SERIAL_PORT_USBVIRTUAL

//Variables para el cálculo de distancia entre 2 puntos
float curr_lat, curr_lon;
int coord = 15;
float los = 0.005f;

//Estructura de datos para determinación de zona más cercana
Quadtree remora_gis = Quadtree();
vector<Data> test_points;

//Cargar quadtree (mapa) a memoria
void load_map(){

}

void setup() {
	pinMode(25, OUTPUT);
	digitalWrite(25, HIGH);
	delay(1000);
	digitalWrite(25, LOW);
	delay(1000);

	Serial.begin(115200);
	while (! Serial);
	Serial.println("======DEBUG======");

	//La Sabana
	remora_gis.insert(-84.110250f, 9.933845f, 0x1E1);
	remora_gis.insert(-84.108877f, 9.938325f, 0x2DF);
	remora_gis.insert(-84.098514f, 9.937848f, 0x2D4);
	remora_gis.insert(-84.098266f, 9.935712f, 0x2D2);
	remora_gis.insert(-84.099593f, 9.935712f, 0x2D6);
	remora_gis.insert(-84.099605f, 9.932733f, 0x2D2);
	//Office
	remora_gis.insert(-84.061049f,9.937767f,0xFFF);

}

void switchCoor(){
	switch (coord){
	case 0:
		curr_lat = 9.936703;
		curr_lon = -84.063194;
		break;
	case 1:
		curr_lat = 9.935228;
		curr_lon = -84.062309;
		break;
	case 2:
		curr_lat = 9.934112;
		curr_lon = -84.062668;
		break;
	case 3:
		curr_lat = 9.935252;
		curr_lon = -84.065079;
		break;
	case 4:
		curr_lat = 9.937017;
		curr_lon = -84.067703;
		break;
	case 5:
		curr_lat = 9.937283;
		curr_lon = -84.070107;
		break;
	case 6:
		curr_lat =9.937508;
		curr_lon =-84.071487;
		break;
	case 7:
		curr_lat =9.936837;
		curr_lon =-84.075058;
		break;
	case 8:
		curr_lat = 9.937518;
		curr_lon =-84.078033;
		break;
	case 9:
		curr_lat = 9.937780;
		curr_lon = -84.079857;
		break;
	case 10:
		curr_lat =9.939762;
		curr_lon =-84.082222;
		break;
	case 11:
		curr_lat =9.940295;
		curr_lon =-84.082733;
		break;
	case 12:
		curr_lat = 9.938722;
		curr_lon = -84.083031;
		break;
	case 13:
		curr_lat = 9.935378;
		curr_lon = -84.083572;
		break;
	case 14:
		curr_lat = 9.934560;
		curr_lon = -84.082550;
		break;
	case 15://Office
		curr_lat = 9.937767;
		curr_lon = -84.061049;
		break;
	}
}

//Consulta si el punto de prueba se encuentra en las áreas dadas
bool point_in_area(const vector<Data>& test_points, float lat, float lon){
	//Serial.println("points");
	bool inside = false;
	for (auto i : test_points){
		//Serial.println("auto");
		inside = i.point_in_area(lat, lon);
		//Serial.print("Colision con area ");
		//Serial.print(i.pos.x);
		//Serial.print(", ");
		//Serial.print(i.pos.y);
		//Serial.print(": ");
		//Serial.println(inside);
		if (inside) break;
	}
	return inside;
}

void loop() {
	if(coord == 16){
		coord = 0;
	}
	switchCoor();
	Serial.print("inicio ");
	Serial.println(coord);
	coord++;

	test_points = remora_gis.nearest_points(curr_lat,curr_lon,los);
	Serial.println(String(point_in_area(test_points, curr_lat, curr_lon)));
	if(point_in_area(test_points, curr_lat, curr_lon)){
		String msg = "{\"lat\":";
		msg += String(curr_lat);
		msg += ",\"lon\":";
		msg += String(curr_lon);
		msg += "}\n";
		Serial.println("VIOLACION DE AREA");
	}
	else{
		Serial.println("Bajo Control");
	}
	delay(2000);
}
