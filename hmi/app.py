# Author: Ammar Aryan Nuha
# Deklarasi library yang digunakan
from flask import Flask, render_template, redirect, url_for, request, get_flashed_messages, flash
from flask_login import LoginManager, UserMixin, login_user, logout_user, login_required, current_user
import dash
import dash_bootstrap_components as dbc
import secrets
import paho.mqtt.client as mqtt
import pandas as pd
import plotly.graph_objects as go
import threading
import requests
from dash import dcc, html, dash_table
from dash.dependencies import Input, Output, State
from pages.co2 import co2_layout
from pages.th_in import th_in_layout
from pages.th_out import th_out_layout  
from pages.par import par_layout    
from pages.windspeed import windspeed_layout    
from pages.rainfall import rainfall_layout  

# Initialize Flask app
server = Flask(__name__)
server.secret_key = secrets.token_hex(32)  # Generates a 64-character hexadecimal key

# Configure Flask-Login
login_manager = LoginManager()
login_manager.init_app(server)

# User data for simplicity (use a database in production)
users = {'admin': {'password': 'admin'}}

class User(UserMixin):
    def __init__(self, id):
        self.id = id

@login_manager.user_loader
def load_user(user_id):
    return User(user_id)

# Flask routes
@server.route('/')
def home():
    return redirect(url_for('login'))

@server.route('/login', methods=['GET', 'POST'])
def login():
    if request.method == 'POST':
        username = request.form['username']
        password = request.form['password']
        if username in users and users[username]['password'] == password:
            user = User(username)
            login_user(user)
            return redirect(url_for('dashboard'))
        
        # Tambahkan flash & redirect untuk POST-REDIRECT-GET
        flash('Invalid credentials')
        return redirect(url_for('login'))

    return render_template('login.html')


@server.route('/dashboard')
@login_required
def dashboard():
    return render_template('dashboard.html', user=current_user.id)

# @server.route('/dash/')
# @login_required
# def dash_home():
#     return redirect('/dash/')

@server.route('/logout')
@login_required
def logout():
    logout_user()
    return redirect(url_for('login'))

# Integrate Dash app
app_dash = dash.Dash(__name__, server=server, url_base_pathname='/dash/', external_stylesheets=[dbc.themes.BOOTSTRAP])

# data storage
data = {'waktu' : [], 'suhu' : [], 'kelembaban' : [], 'co2' : [],
        'windspeed' : [], 'rainfall' : []}

# MQTT Configuration
BROKER = "9a59e12602b646a292e7e66a5296e0ed.s1.eu.hivemq.cloud"
PORT = 8883
USERNAME = "testing"
PASSWORD = "Testing123"
TOPIC_SUHU = "esp32/suhu"
TOPIC_KELEMBABAN = "esp32/kelembaban"
TOPIC_CO2 = "esp32/co2"
TOPIC_WINDSPEED = "esp32/windspeed"
TOPIC_RAINFALL = "esp32/rainfall"

# MQTT Callback
def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print("Connected to HiveMQ Broker")
        client.subscribe([(TOPIC_SUHU, 0), (TOPIC_KELEMBABAN, 0),
                          (TOPIC_CO2, 0), (TOPIC_WINDSPEED, 0), (TOPIC_RAINFALL, 0)])  # Subscribe ke topik suhu & kelembaban
    else:
        print(f"Failed to connect, return code {rc}")

def on_message(client, userdata, msg):
    global data
    # print(f"Received message: {msg.topic} - {msg.payload.decode()}")
    try:
        value = float(msg.payload.decode())
        time = pd.Timestamp.now().strftime('%H:%M:%S')
        if msg.topic == TOPIC_SUHU:
            data['waktu'].append(time)
            data['suhu'].append(value)
        elif msg.topic == TOPIC_KELEMBABAN:
            data['kelembaban'].append(value)
        elif msg.topic == TOPIC_CO2:
            data['co2'].append(value)
        elif msg.topic == TOPIC_WINDSPEED:
            data['windspeed'].append(value)
        elif msg.topic == TOPIC_RAINFALL:
            data['rainfall'].append(value)
        
        if len(data['waktu']) > 20:
            data['waktu'].pop(0)
            data['suhu'].pop(0)
            data['kelembaban'].pop(0)
            data['co2'].pop(0)
            data['windspeed'].pop(0)
            data['rainfall'].pop(0)
    
    except Exception as e:
        print(f"Error processing MQTT message: {e}")

# MQTT Client
client = mqtt.Client()
client.username_pw_set(USERNAME, PASSWORD)
client.tls_set()
client.on_connect = on_connect
client.on_message = on_message
client.connect(BROKER, PORT, 60)

# Jalankan MQTT dalam thread
threading.Thread(target=client.loop_forever, daemon=True).start()

app_dash.layout = html.Div([
    # CSS styles for the app
    html.Link(rel='stylesheet', href='/static/style.css'),

    # NAVBAR
    html.Div([
        html.Div("MICROCLIMATE SYSTEM DASHBOARD", className="navbar-title"),
        html.Img(src="/static/icon/notification.svg", className="notification-icon"),
    ], className="d-flex justify-content-between align-items-center p-3 border-bottom navbar-full"),

    # TAMPILAN PARAMETER SENSOR GRID
    html.Div([
        dbc.Row([
            dbc.Col(html.Div([
                html.Div([
                    html.Div([
                        html.Img(src="/static/icon/temperature.svg", className="param-icon me-2"),
                        html.Span("TEMPERATURE IN", className="param-title me-2"),
                        html.Span(id="suhu-display-indoor", className="param-value")
                    ], className="d-flex align-items-center mb-2"),
                    html.Div([
                        html.Img(src="/static/icon/humidity.svg", className="param-icon me-2"),
                        html.Span("HUMIDITY IN", className="param-title me-2"),
                        html.Span(id="kelembaban-display-indoor", className="param-value")
                    ], className="d-flex align-items-center")
                ])
            ], className="param-card"), width=4),

            dbc.Col(html.Div([
                html.Div([
                    html.Img(src="/static/icon/sun.svg", className="param-icon me-2"),
                    html.Span("PAR", className="param-title me-2"),
                    html.Span("15 µmol·m⁻²·s⁻¹", className="param-value")
                ], className="d-flex align-items-center")
            ], className="param-card"), width=4),

            dbc.Col(html.Div([
                html.Div([
                    html.Img(src="/static/icon/co.svg", className="param-icon me-2"),
                    html.Span("CO2", className="param-title me-2"),
                    html.Span(id="co2-display", className="param-value")
                ], className="d-flex align-items-center")
            ], className="param-card"), width=4),
        ], className="mb-3"),

        dbc.Row([
            dbc.Col(html.Div([
                html.Div([
                    html.Div([
                        html.Img(src="/static/icon/temperature.svg", className="param-icon me-2"),
                        html.Span("TEMPERATURE OUT", className="param-title me-2"),
                        html.Span(id="suhu-display-outdoor", className="param-value")
                    ], className="d-flex align-items-center mb-2"),
                    html.Div([
                        html.Img(src="/static/icon/humidity.svg", className="param-icon me-2"),
                        html.Span("HUMIDITY OUT", className="param-title me-2"),
                        html.Span(id="kelembaban-display-outdoor", className="param-value")
                    ], className="d-flex align-items-center")
                ])
            ], className="param-card"), width=4),

            dbc.Col(html.Div([
                html.Div([
                    html.Img(src="/static/icon/windspeed.svg", className="param-icon me-2"),
                    html.Span("WINDSPEED", className="param-title me-2"),
                    html.Span(id="windspeed-display", className="param-value")
                ], className="d-flex align-items-center")
            ], className="param-card"), width=4),

            dbc.Col(html.Div([
                html.Div([
                    html.Img(src="/static/icon/rainfall.svg", className="param-icon me-2"),
                    html.Span("RAINFALL", className="param-title me-2"),
                    html.Span(id="rainfall-display", className="param-value")
                ], className="d-flex align-items-center")
            ], className="param-card"), width=4),
        ])
    ], className="container my-3"),

    # TAMPILAN BOTTOM GRID
    html.Div([
        dbc.Row([
            dbc.Col(html.Img(src="/static/img/gh.jpg", className="greenhouse-img"), width=6),
            dbc.Col([

                # # Alarm Section
                # html.Div([
                #     dbc.Row([
                #         dbc.Col([html.Div(className="alarm-dot red"), html.Div("T&H", className="text-center")]),
                #         dbc.Col([html.Div(className="alarm-dot yellow"), html.Div("PAR", className="text-center")]),
                #         dbc.Col([html.Div(className="alarm-dot green"), html.Div("CO2", className="text-center")]),
                #         dbc.Col([html.Div(className="alarm-dot red"), html.Div("VOLT", className="text-center")]),
                #         dbc.Col([html.Div(className="alarm-dot green"), html.Div("CURRENT", className="text-center")]),
                #         dbc.Col([html.Div(className="alarm-dot yellow"), html.Div("POWER", className="text-center")])
                #     ], className="text-center mb-3")
                # ]),

                # Table Section
                html.Div([
                    html.H4("Real Time Data", className="text-center mb-2"),
                    dash_table.DataTable(
                        columns=[
                            {"name": i, "id": i} for i in [
                                "Temp (°C)", "Humidity (%)", "PAR (2-s-1)", "CO2 (PPM)", "Volt (V)", "Current (A)", "Power (Watt)"
                            ]
                        ],
                        data=[],
                        style_table={'overflowX': 'auto'},
                        style_cell={"textAlign": "center"}
                    )
                ], className="data-table mb-3"),

                # Button Section
                html.Div([
                    html.Button("SETTING", className="btn btn-secondary m-1"),
                    html.Button("T&H INDOOR", className="btn btn-secondary m-1"),
                    html.Button("PAR", className="btn btn-secondary m-1"),
                    html.Button("CO2", className="btn btn-secondary m-1"),
                    html.Button("T&H OUTDOOR", className="btn btn-secondary m-1"),
                    html.Button("WINDSPEED", className="btn btn-secondary m-1"),
                    html.Button("RAINFALL", className="btn btn-secondary m-1"),
                    html.Button("LOGOUT", id="logout-button", className="btn btn-dark m-1"),
                    dcc.Location(id="logout-redirect", refresh=True)  # Handles redirection
                ], className="d-flex flex-wrap justify-content-end")
            ], width=6)
        ], className="g-2")
    ], className="container mb-5"),

    dcc.Interval(id='interval', interval=1100, n_intervals=0)
])

# Callback Real Time Trend
@app_dash.callback(
    [Output('suhu-display-indoor', 'children'),
     Output('kelembaban-display-indoor', 'children'),
     Output('co2-display', 'children'),
     Output('windspeed-display', 'children'),
     Output('rainfall-display', 'children')],
    [Input('interval', 'n_intervals')]
)
# update dashboard Real Time Trend
def update_dashboard(n):
    suhu = data['suhu'][-1] if len(data['suhu']) == len(data['waktu']) and data['suhu'] else 0
    kelembaban = data['kelembaban'][-1] if len(data['kelembaban']) == len(data['waktu']) and data['kelembaban'] else 0
    co2 = data['co2'][-1] if len(data['co2']) == len(data['waktu']) and data['co2'] else 0
    windspeed = data['windspeed'][-1] if len(data['windspeed']) == len(data['waktu']) and data['windspeed'] else 0
    rainfall = data['rainfall'][-1] if len(data['rainfall']) == len(data['waktu']) and data['rainfall'] else 0

    suhu_display = f" {suhu}°C"
    kelembaban_display = f" {kelembaban}%"
    co2_display = f" {co2}PPM"
    windspeed_display = f" {windspeed}m/s"
    rainfall_display = f" {rainfall}mm"
    
    return suhu_display, kelembaban_display, co2_display, windspeed_display, rainfall_display

# Callbacks for Dash app
@app_dash.callback(
    Output("logout-redirect", "href"),
    Input("logout-button", "n_clicks")
)
def logout_redirect(n_clicks):
    if n_clicks:
        return "/logout"  # Redirects to Flask route
    return None

# Run server
if __name__ == '__main__':
    server.run(debug=True)
