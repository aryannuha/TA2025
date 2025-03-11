import dash
from dash import dcc, html
from dash.dependencies import Input, Output
import paho.mqtt.client as mqtt
import pandas as pd
import plotly.graph_objs as go
import threading

# Inisialisasi Dash
app = dash.Dash(__name__, external_stylesheets=['/assets/style.css'], title="Dashboard Microclimate")
server = app.server

# Data Storage
data = {'waktu': [], 'suhu': [], 'kelembaban': []}

# MQTT Configuration
BROKER = "9a59e12602b646a292e7e66a5296e0ed.s1.eu.hivemq.cloud"
PORT = 8883
USERNAME = "testing"
PASSWORD = "Testing123"
TOPIC_SUHU = "esp32/suhu"
TOPIC_KELEMBABAN = "esp32/kelembaban"

# MQTT Callback
def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print("Connected to HiveMQ Broker")
        client.subscribe([(TOPIC_SUHU, 0), (TOPIC_KELEMBABAN, 0)])  # Subscribe ke topik suhu & kelembaban
    else:
        print(f"Failed to connect, return code {rc}")

def on_message(client, userdata, msg):
    global data
    # print(f"Received message: {msg.topic} - {msg.payload.decode()}")
    value = float(msg.payload.decode())
    time = pd.Timestamp.now().strftime('%H:%M:%S')
    if msg.topic == TOPIC_SUHU:
        data['waktu'].append(time)
        data['suhu'].append(value)
    elif msg.topic == TOPIC_KELEMBABAN:
        data['kelembaban'].append(value)
    
    if len(data['waktu']) > 20:
        data['waktu'].pop(0)
        data['suhu'].pop(0)
        data['kelembaban'].pop(0)

# MQTT Client
client = mqtt.Client()
client.username_pw_set(USERNAME, PASSWORD)
client.tls_set()
client.on_connect = on_connect
client.on_message = on_message
client.connect(BROKER, PORT, 60)

# Jalankan MQTT dalam thread
threading.Thread(target=client.loop_forever, daemon=True).start()

# Layout Dashboard
app.layout = html.Div([
    html.H1("DASHBOARD MONITORING MICROCLIMATE"),

    html.Div([
        # Bagian kiri
        html.Div([
            html.Div([
                html.Img(src='/assets/img/thermometer.png', className='icon'),
                html.Div(id='suhu-display', className='data-box')
            ],className='data-item'),

            html.Div([
                html.Img(src='/assets/img/humidity.png', className='icon'),
                html.Div(id='kelembaban-display', className='data-box')
            ],className='data-item'),

            html.Div([
                html.Table([
                    html.Thead([
                        html.Tr([html.Th("Waktu"), html.Th("Suhu (°C)"), html.Th("Kelembaban (%)")])
                    ]),
                    html.Tbody(id='table-body')
                ], className='data-table'),
            ], className='table-container'),

            html.Button("Move to PAR", id='btn-par', className='btn-par'),
        ], className='left-panel'),

        # Bagian kanan (grafik)
        html.Div([
            dcc.Graph(id='suhu-graph', className='grafik'),
            dcc.Graph(id='kelembaban-graph', className='grafik'),
        ], className='right-panel'),

    ], className='main-container'),

    dcc.Interval(id='interval', interval=2000, n_intervals=0)
])

# Callback
@app.callback(
    [Output('suhu-display', 'children'),
     Output('kelembaban-display', 'children'),
     Output('suhu-graph', 'figure'),
     Output('kelembaban-graph', 'figure'),
     Output('table-body', 'children')],
    [Input('interval', 'n_intervals')]
)

# update dashboard
def update_dashboard(n):
    suhu = data['suhu'][-1] if len(data['suhu']) == len(data['waktu']) and data['suhu'] else 0
    kelembaban = data['kelembaban'][-1] if len(data['kelembaban']) == len(data['waktu']) and data['kelembaban'] else 0

    suhu_display = f"Suhu Udara: {suhu}°C"
    kelembaban_display = f"Kelembapan Udara: {kelembaban}%"
    
    fig_suhu = go.Figure(go.Scatter(x=data['waktu'], y=data['suhu'], mode='lines+markers', name='suhu'))
    fig_suhu.update_layout(title='Suhu Udara', xaxis_title='Waktu', yaxis_title='Suhu (°C)')
    
    fig_kelembaban = go.Figure(go.Scatter(x=data['waktu'], y=data['kelembaban'], mode='lines+markers', name='kelembaban'))
    fig_kelembaban.update_layout(title='Kelembaban Udara', xaxis_title='Waktu', yaxis_title='Kelembaban (%)')
    
    table_rows = [html.Tr([html.Td(w), html.Td(s), html.Td(k)]) 
              for w, s, k in zip(data['waktu'], data['suhu'], data['kelembaban'])]
    
    return suhu_display, kelembaban_display, fig_suhu, fig_kelembaban, table_rows

if __name__ == '__main__':
    app.run_server(debug=True)
