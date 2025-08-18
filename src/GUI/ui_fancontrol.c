/*
 * Copyright (C) 2025 xsys061@gmail.com
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301 USA
 */











#include <stdio.h>
#include <unistd.h>
//#include <ui.h>
#include "ui_fancontrol.h"
#include <psensor/psensor.h>
#include <psensor/pio.h>
#include <glib.h>
#include <fan_control_utils.h>




int FanPWM;
int FanLastPwm[100];
int combx_i;
psensor_fan *PCFans;
GtkScale *FanPwmSL;
GtkDialog *dg;



void TestFanFct(GtkButton* btn, gpointer dat)
{
  psensor_test_fan(PCFans->pwmFiles[combx_i]);
}

void SwitchFanList(GtkComboBoxText *combx, gpointer user_data)
{
  combx_i = gtk_combo_box_get_active(GTK_COMBO_BOX(combx));
  gtk_range_set_value(GTK_RANGE(FanPwmSL), FanLastPwm[combx_i]);
}

void GetPWMVal(GtkRange *rg, gpointer user_data)
{
  FanPWM = gtk_range_get_value(rg);
  psensor_fan_set_pwm(PCFans->pwmFiles[combx_i], FanPWM);
}

void fanGtrlDlg(struct ui_psensor *ui)
{
  gint result;
  struct config *cfg;
  GtkBuilder *builder;
  guint ok;
  GError *err = NULL;
  GtkSpinButton *FanPwmSB;
  GtkComboBoxText *FanList;
  GtkButton *testFan;

  cfg = ui -> config;

  PCFans = psensor_detectFans();

  builder = gtk_builder_new();

                                          //located in: /usr/local/share/psensor
  ok = gtk_builder_add_from_file(builder, "/usr/local/share/psensor/psensor-fan-controller.glade", &err);

  if(!ok)
  {
    log_printf(LOG_ERR, err -> message);
    g_error_free(err);
    return;
  }

  dg = GTK_DIALOG(gtk_builder_get_object(builder, "dialog3"));

  FanList = GTK_COMBO_BOX_TEXT(gtk_builder_get_object(builder, "CBXFanList"));
  g_signal_connect(FanList, "changed", G_CALLBACK(SwitchFanList), NULL);

  FanPwmSL = GTK_SCALE(gtk_builder_get_object(builder, "SetFanPwmSL"));

  testFan = GTK_BUTTON(gtk_builder_get_object(builder, "fanTest"));

  char temp[550];

  for (int i = 0; i < PCFans->fanInputCount; i++)
  {
    sprintf(temp, "Fan %d", i);
    gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(FanList), NULL, temp);
    psensor_enable_fan_pwm(PCFans->pwmEnableFiles[i], 1);
    FanLastPwm[i] = psensor_get_last_pwm(PCFans->pwmFiles[i]);
    //printf("Last pwm's at: %s | %d | %s\n", PCFans->pwmFiles[i], FanLastPwm[i], temp);
  }


  gtk_combo_box_set_active(GTK_COMBO_BOX(FanList), 0);

  g_signal_connect(FanPwmSL, "value-changed", G_CALLBACK(GetPWMVal), NULL);
  g_signal_connect(testFan, "clicked", G_CALLBACK(TestFanFct), NULL);


  gtk_builder_connect_signals(builder, NULL);

  result = gtk_dialog_run(dg);


  g_object_unref(G_OBJECT(builder));
  gtk_widget_destroy(GTK_WIDGET(dg));
}

void FreePCFansArrays()
{
  for (int i = 0; i < PCFans->fanInputCount; i++)
  {
    free(PCFans->pwmFiles[i]);
    free(PCFans->pwmEnableFiles[i]);
  }
  free(PCFans->pwmFiles);
  free(PCFans->pwmEnableFiles);
  free(PCFans);
}
