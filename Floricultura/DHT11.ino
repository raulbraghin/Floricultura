float FuncHumidade() {

  float h = dht.readHumidity();

  if (isnan(h)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return 0.00;
  } else {

    return h;
  }
}

float FuncTemperatura() {

  float t = dht.readTemperature();

  if (isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return 0.00;
  } else {

    return t;
  }
}