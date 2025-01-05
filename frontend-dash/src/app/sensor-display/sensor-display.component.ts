import { Component, OnInit } from '@angular/core';
import { CommonModule } from '@angular/common';
import { SensorDataService } from '../sensor-data.service';

@Component({
  selector: 'app-sensor-display',
  standalone: true,
  imports: [CommonModule],
  templateUrl: './sensor-display.component.html',
  styleUrls: ['./sensor-display.component.css'],
})
export class SensorDisplayComponent implements OnInit {
  sensorData: any;
  dataCount: any;
  usersPoints: any;
  usersPointsPerDay: any;
  rewardLog: any;
  userCount: any;

  constructor(private sensorDataService: SensorDataService) { }

  ngOnInit(): void {
    this.fetchData('sensorData', this.sensorDataService.getSensorData(), true);
    this.fetchData('dataCount', this.sensorDataService.getDataCount());
    this.fetchData('usersPoints', this.sensorDataService.getUsersPoints());
    this.fetchData('usersPointsPerDay', this.sensorDataService.getUsersPointsPerDay());
    this.fetchData('rewardLog', this.sensorDataService.getRewardLog(),true);
    this.fetchData('userCount', this.sensorDataService.getUserCount())
  }

  /**
   * Centralized subscription logic for handling observable data.
   * @param property Component property to assign the data to.
   * @param observable The observable to subscribe to.
   * @param reverse Optional flag to reverse the data array (default: false).
   */
  private fetchData(property: keyof this, observable: any, reverse: boolean = false): void {
    observable.subscribe({
      next: (data: any) => {
        // Convert property to string for dynamic assignment
        (this as any)[String(property)] = reverse && Array.isArray(data) ? data.reverse() : data;
      },
      error: (error: any) => {
        console.error(`Error fetching ${String(property)}:`, error);
      },
    });
  }

}
