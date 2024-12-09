import { Component } from '@angular/core';
import { RouterOutlet } from '@angular/router';
import { SensorDisplayComponent } from './sensor-display/sensor-display.component';

@Component({
  selector: 'app-root',
  standalone: true,
  imports: [RouterOutlet, SensorDisplayComponent], 
  templateUrl: './app.component.html',
  styleUrl: './app.component.css'
})
export class AppComponent {
  title = 'frontend-dash';
}
