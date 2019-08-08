#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Mon Aug  5 13:51:15 2019

@author: mariana góis
"""

#-------------------------------------------------------------------BIBLIOTECAS

import serial
import time
import threading
from tkinter import *
from functools import partial
import serial.tools.list_ports

from matplotlib.backends.backend_tkagg import (
    FigureCanvasTkAgg, NavigationToolbar2Tk)
# Implement the default Matplotlib key bindings.
from matplotlib.backend_bases import key_press_handler
from matplotlib.figure import Figure

import numpy as np

#---------------------------------------------------------------------VARIÁVEIS

desligado = '#c0392b'
ligado = '#2ecc71'
cordefundo = '#ecf0f1'
portastr = '/dev/ttyUSB2'
global scheduler

#-------------------------------------------------------------FUNÇÃO DOS BOTÕES
def serial_ports():
    comlist = serial.tools.list_ports.comports()
    connected = []
    for element in comlist:
        connected.append(element.device)
    return connected

def arduinoonoff():
    if btarduino["text"] == "PLACA ON":
        btarduino["text"] = "PLACA OFF"
        btarduino["bg"] = desligado
        conexao.close()
    else:
        conexao.open()
        if conexao.is_open == False:
            print("Sem conexão. Tente outra porta de comunicação")
        else:
            btarduino["text"] = "PLACA ON"
            btarduino["bg"] = ligado
        
def mudarporta():
    lbporta["text"] = " PORTA: " + edporta.get()
    portastr = edporta.get()
    conexao.port = portastr
    
def mudarsetpoint():
    lbtempsp["text"] = edsetpoint.get()
    # setpoint = edsetpoint.get()
        
def ligabombareator():
    if btreator["bg"] == desligado:
        conexao.write(b'a') # liga
        btreator["bg"] = ligado
    else:
        conexao.write(b'b') # desliga
        btreator["bg"] = desligado

def ligabombaresfriador():
    if bttrocador["bg"] == desligado:
        conexao.write(b'c') # liga
        bttrocador["bg"] = ligado
    else:
        conexao.write(b'd') # desliga
        bttrocador["bg"] = desligado
    
def ligasolenoide():
    if btatomizador["bg"] == desligado:
        conexao.write(b'l') # liga
        btatomizador["bg"] = ligado
    else:
        conexao.write(b'm') # desliga
        btatomizador["bg"] = desligado
'''
def pegartemp():
    conexao.write(b'e')
    temperatura = conexao.readline()
    temperatura = str(temperatura, 'utf-8')
    print("Temperatura: ", temperatura)
'''
def aquisicaodados():
    if btdados["bg"] == desligado:
        btdados["bg"] = ligado
        t1 = threading.Thread(target=adquirirdados)
        t1.daemon = True
        t1.start()
    else:
        btdados["bg"] = desligado
        t1.stop()
        
def adquirirdados():
    while True:
        arduinostring = conexao.readline()
        arduinostring = str(arduinostring, 'utf-8')
        dadosarray = arduinostring.split(',')
        tempreator = float(dadosarray[0])
        tempbanho = float(dadosarray[1])
        TemperaturaReator.append(tempreator)
        TemperaturaBanho.append(tempbanho)
        lbtemp1["text"] = tempreator
        lbtemp2["text"] = tempbanho

def sair():
    conexao.close()
    janela.quit()     
    janela.destroy()
    
#def fazerfigura():
    
#-------------------------------------------------------CONFIGURAÇÕES DA JANELA

janela = Tk() # criando
janela.title("Reator de atomização") # nomeando
janela["bg"] = cordefundo # cor de fundo 
#janela.state("zoomed") # fullscreen
#janela.overrideredirect(1) # remove a barra de cima
#janela.resizable(0,0) # remove maximização

janela.geometry("925x600+200+200") # LarguraxAltura+E+T

#-----------------------------------------------------------------PORTA ARDUINO

conexao = serial.Serial()
conexao.baudrate = 9600
porta = serial_ports()
portastr = str(porta[0])
conexao.port = portastr

#------------------------------------------------------------------------BOTÕES

btdados = Button(janela, width=18, height=2, text="AQUISIÇÃO DE DADOS", 
                 bg=desligado, command=aquisicaodados, fg="white")
btdados.grid(row=0,column=3, sticky="nsew")

btreator = Button(janela, width=18, height=2, text="BOMBA DO REATOR", 
                  bg=desligado, command=ligabombareator, fg="white")
btreator.grid(row=0,column=4, sticky="nsew")

bttrocador = Button(janela, width=18, height=2, text="BOMBA DO TROCADOR", 
                    bg=desligado, command=ligabombaresfriador, fg="white")
bttrocador.grid(row=0,column=5, sticky="nsew")

btcompressor = Button(janela, width=18, height=2, text="COMPRESSOR", 
                      bg=desligado, fg="white")
btcompressor.grid(row=0,column=6, sticky="nsew")

btatomizador = Button(janela, width=18, height=2, text="ATOMIZADOR", 
                      bg=desligado, command=ligasolenoide, fg="white")
btatomizador.grid(row=0,column=7, sticky="nsew")

btarduino = Button(janela, width=18, height=2, text="PLACA OFF", 
                   bg=desligado, command=arduinoonoff, fg="white")
btarduino.grid(row=0,column=0, sticky="nsew", columnspan=3)

lbporta = Label(janela, text="PORTA: " + portastr, bg=cordefundo)
lbporta.grid(row=1,column=0, sticky="nsew", columnspan=3)

edporta = Entry(janela)
edporta.grid(row=2,column=0, sticky="nsew", columnspan=3)

btporta = Button(janela, width=18, height=2, text="MUDAR PORTA", 
                 command=mudarporta)
btporta.grid(row=3,column=0, sticky="nsew", columnspan=3)

#----------------------------------------------------------------------CONTROLE

lbcontrole = Label(janela, text="CONTROLE", bg=cordefundo, 
                   font=("Helvetica", 20))
lbcontrole.grid(row=4,column=0, sticky="nsew", columnspan=3)

btporta = Button(janela, width=18, height=2, text="AUTOMÁTICO", 
                 bg=desligado, fg="white")
btporta.grid(row=5,column=0, sticky="nsew", columnspan=3)

#-------------------------------------------------------------------TEMPERATURA

lbsetpoint = Label(janela, text="SetPoint: ", bg=cordefundo,
                   font=("Helvetica", 16))
lbsetpoint.grid(row=6,column=0, columnspan=2)

lbtempsp = Label(janela, text="30.00", bg=cordefundo, font=("Helvetica", 16))
lbtempsp.grid(row=6,column=2)

edsetpoint = Entry(janela)
edsetpoint.grid(row=7,column=0, sticky="nsew", columnspan=3)

btsetpoint = Button(janela, width=18, height=2, text="SET-POINT", 
                 command=mudarsetpoint, bg=desligado, fg="white")
btsetpoint.grid(row=8,column=0, sticky="nsew", columnspan=3)

lbrotulotemp1 = Label(janela, text="Treator:", bg=cordefundo,
                      font=("Helvetica", 16))
lbrotulotemp1.grid(row=9,column=0, columnspan=2)

lbtemp1 = Label(janela, text="22.00", bg=cordefundo, font=("Helvetica", 16))
lbtemp1.grid(row=9,column=2)

lbrotulotemp2 = Label(janela, text="Tbanho:", bg=cordefundo,
                      font=("Helvetica", 16))
lbrotulotemp2.grid(row=10, column=0, columnspan=2)

lbtemp2 = Label(janela, text="20.00", bg=cordefundo, font=("Helvetica", 16))
lbtemp2.grid(row=10,column=2)
#-------------------------------------------------------------------------DADOS

lbtempo = Label(janela, text="Tempo", bg=cordefundo, font=("Helvetica", 12))
lbtempo.grid(row=11,column=0)

lbtempo = Label(janela, text="Tbanho", bg=cordefundo, font=("Helvetica", 12))
lbtempo.grid(row=11,column=1)

lbtempo = Label(janela, text="Treator", bg=cordefundo, font=("Helvetica", 12))
lbtempo.grid(row=11,column=2)

for x in range(12,22):
    for y in range(0,3):
        lbtempo = Label(janela, text="-", bg=cordefundo, 
                        font=("Helvetica", 12))
        lbtempo.grid(row=x,column=y, sticky="nsew")

#-----------------------------------------------------------------------GRÁFICO
fig = Figure(figsize=(5, 4), dpi=100)
t = np.arange(0, 3, .01)
fig.add_subplot(111).plot(t, 2 * np.sin(2 * np.pi * t))

canvas = FigureCanvasTkAgg(fig, master=janela)  # A tk.DrawingArea.
canvas.draw()
canvas.get_tk_widget().grid(row=1,column=3, columnspan=5, rowspan=22, sticky="nsew")
'''
lbgrafico = Label(janela, text="GRÁFICO", bg="blue",
                   font=("Helvetica", 16))
lbgrafico.grid(row=1,column=3, columnspan=5, rowspan=22, sticky="nsew")
'''
#-------------------------------------------------------------JANELA RESPONSIVA

for x in range(3,8):
    Grid.columnconfigure(janela, x, weight=1)
for y in range(23):
    Grid.rowconfigure(janela, y, weight=1)

#-------------------------------------------------------------------------FINAL
btfinal= Button(janela, text="SAIR", command=sair)
btfinal.grid(row=22,column=0, columnspan=8, sticky="nsew")

janela.mainloop() # enquanto a janela estiver aberta não executa nada depois
