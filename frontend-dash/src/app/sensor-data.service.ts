import { Injectable } from '@angular/core';
import { HttpClient } from '@angular/common/http';
import { Observable } from 'rxjs';

@Injectable({
  providedIn: 'root',
})
export class SensorDataService {
  private apiUrl = 'http://127.0.0.1:8080/api/data';  // Update this if necessary

  constructor(private http: HttpClient) {}

  // This method fetches the sensor data from the backend API
  getSensorData(): Observable<any> {
    return this.http.get<any>(this.apiUrl);
  }
}
