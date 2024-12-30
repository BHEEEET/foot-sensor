import { Routes } from '@angular/router';
import { ContactComponent } from './contact/contact.component';
import { SensorDisplayComponent } from './sensor-display/sensor-display.component';

export const routes: Routes = [
  { path: '', component: SensorDisplayComponent },
  { path: 'contact', component: ContactComponent }
];
