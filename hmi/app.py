import dash
from dash import html, dcc, dash_table, Input, Output, callback
import dash_bootstrap_components as dbc

app = dash.Dash(__name__, external_stylesheets=[dbc.themes.BOOTSTRAP])

app.layout = html.Div([
    # NAVBAR
    html.Div([
        html.Div("MICROCLIMATE STATION INDOOR", className="navbar-title"),
        html.Img(src="/assets/icon/notification.svg", className="notification-icon"),
    ], className="d-flex justify-content-between align-items-center p-3 border-bottom navbar-full"),

    # PARAMETER GRID
    html.Div([
        dbc.Row([
            dbc.Col(html.Div([
                html.Div([
                    html.Div([
                        html.Img(src="/assets/icon/temperature.svg", className="param-icon me-2"),
                        html.Span("TEMPERATURE", className="param-title me-2"),
                        html.Span("20 °C", className="param-value")
                    ], className="d-flex align-items-center mb-2"),
                    html.Div([
                        html.Img(src="/assets/icon/humidity.svg", className="param-icon me-2"),
                        html.Span("HUMIDITY", className="param-title me-2"),
                        html.Span("70 %", className="param-value")
                    ], className="d-flex align-items-center")
                ])
            ], className="param-card"), width=4),

            dbc.Col(html.Div([
                html.Div([
                    html.Img(src="/assets/icon/sun.svg", className="param-icon me-2"),
                    html.Span("PAR", className="param-title me-2"),
                    html.Span("15 µmol·m⁻²·s⁻¹", className="param-value")
                ], className="d-flex align-items-center")
            ], className="param-card"), width=4),

            dbc.Col(html.Div([
                html.Div([
                    html.Img(src="/assets/icon/co.svg", className="param-icon me-2"),
                    html.Span("CO2", className="param-title me-2"),
                    html.Span("400 PPM", className="param-value")
                ], className="d-flex align-items-center")
            ], className="param-card"), width=4),
        ], className="mb-3"),

        dbc.Row([
            dbc.Col(html.Div([
                html.Div([
                    html.Img(src="/assets/icon/voltage.svg", className="param-icon me-2"),
                    html.Span("VOLTAGE", className="param-title me-2"),
                    html.Span("10 V", className="param-value")
                ], className="d-flex align-items-center")
            ], className="param-card"), width=4),

            dbc.Col(html.Div([
                html.Div([
                    html.Img(src="/assets/icon/resistors.svg", className="param-icon me-2"),
                    html.Span("CURRENT", className="param-title me-2"),
                    html.Span("5 A", className="param-value")
                ], className="d-flex align-items-center")
            ], className="param-card"), width=4),

            dbc.Col(html.Div([
                html.Div([
                    html.Img(src="/assets/icon/power.svg", className="param-icon me-2"),
                    html.Span("POWER", className="param-title me-2"),
                    html.Span("25 WATT", className="param-value")
                ], className="d-flex align-items-center")
            ], className="param-card"), width=4),
        ])
    ], className="container my-3"),

    # BOTTOM GRID
    html.Div([
        dbc.Row([
            dbc.Col(html.Img(src="/assets/img/gh.jpg", className="greenhouse-img"), width=6),
            dbc.Col([

                # Alarm Section
                html.Div([
                    dbc.Row([
                        dbc.Col([html.Div(className="alarm-dot red"), html.Div("T&H", className="text-center")]),
                        dbc.Col([html.Div(className="alarm-dot yellow"), html.Div("PAR", className="text-center")]),
                        dbc.Col([html.Div(className="alarm-dot green"), html.Div("CO2", className="text-center")]),
                        dbc.Col([html.Div(className="alarm-dot red"), html.Div("VOLT", className="text-center")]),
                        dbc.Col([html.Div(className="alarm-dot green"), html.Div("CURRENT", className="text-center")]),
                        dbc.Col([html.Div(className="alarm-dot yellow"), html.Div("POWER", className="text-center")])
                    ], className="text-center mb-3")
                ]),

                # Table Section
                html.Div([
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
                    html.Button("SETTING", className="btn btn-secondary me-2"),
                    html.Button("LOGOUT", className="btn btn-dark")
                ], className="d-flex justify-content-end")
            ], width=6)
        ], className="g-2")
    ], className="container mb-5")
])

if __name__ == '__main__':
    app.run(debug=True)
