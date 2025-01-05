import { Injectable } from '@angular/core';
import { HttpClient } from '@angular/common/http';
import { Observable } from 'rxjs';
import { catchError } from 'rxjs/operators';

@Injectable({
  providedIn: 'root',
})
export class SensorDataService {
  private baseApiUrl = 'http://127.0.0.1:8080/api'; // Base API URL
  private endpoints = {
    data: `${this.baseApiUrl}/data`,
    dataCount: `${this.baseApiUrl}/data/count`,
    rewardPoints: `${this.baseApiUrl}/reward/total`,
    rewardPointsPerDay: `${this.baseApiUrl}/reward/total-per-day`,
    rewardLog: `${this.baseApiUrl}/reward`,
    userDayCount: `${this.baseApiUrl}/data/user-count`
  };

  constructor(private http: HttpClient) {}

  // Fetch all sensor data
  getSensorData(): Observable<any> {
    return this.http.get<any>(this.endpoints.data).pipe(
      catchError(this.handleError('getSensorData'))
    );
  }

  // Fetch the count of sensor data
  getDataCount(): Observable<any> {
    return this.http.get<any>(this.endpoints.dataCount).pipe(
      catchError(this.handleError('getDataCount'))
    );
  }

  // Fetch total user points
  getUsersPoints(): Observable<any> {
    return this.http.get<any>(this.endpoints.rewardPoints).pipe(
      catchError(this.handleError('getUsersPoints'))
    );
  }

  // Fetch total user points per day
  getUsersPointsPerDay(): Observable<any> {
    return this.http.get<any>(this.endpoints.rewardPointsPerDay).pipe(
      catchError(this.handleError('getUsersPointsPerDay'))
    );
  }

  // Fetch user reward log
  getRewardLog(): Observable<any> {
    return this.http.get<any>(this.endpoints.rewardLog).pipe(
      catchError(this.handleError('getRewardLog'))
    );
  }

    // Fetch user reward log
    getUserCount(): Observable<any> {
      return this.http.get<any>(this.endpoints.userDayCount).pipe(
        catchError(this.handleError('getUserCount'))
      );
    }

  // Centralized error handler
  private handleError(operation = 'operation') {
    return (error: any): Observable<never> => {
      console.error(`${operation} failed:`, error);
      throw error; // Rethrow the error after logging it
    };
  }
}
