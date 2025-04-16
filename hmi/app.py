from flask import Flask, render_template, redirect, url_for, request
from flask_login import LoginManager, UserMixin, login_user, logout_user, login_required, current_user
import dash
import dash_bootstrap_components as dbc
from dash import dcc, html, dash_table
from dash.dependencies import Input, Output, State

# Initialize Flask app
server = Flask(__name__)
server.secret_key = 'your_secret_key'

# Configure Flask-Login
login_manager = LoginManager()
login_manager.init_app(server)

# User data for simplicity (use a database in production)
users = {'admin': {'password': 'password'}}

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
        return 'Invalid credentials', 401
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
app_dash.layout = html.Div([
    # CSS styles for the app
    html.Link(rel='stylesheet', href='/static/style.css'),

    # NAVBAR
    html.Div([
        html.Div("MICROCLIMATE STATION INDOOR", className="navbar-title"),
        html.Img(src="/static/icon/notification.svg", className="notification-icon"),
    ], className="d-flex justify-content-between align-items-center p-3 border-bottom navbar-full"),

    # PARAMETER GRID
    html.Div([
        dbc.Row([
            dbc.Col(html.Div([
                html.Div([
                    html.Div([
                        html.Img(src="/static/icon/temperature.svg", className="param-icon me-2"),
                        html.Span("TEMPERATURE", className="param-title me-2"),
                        html.Span("20 °C", className="param-value")
                    ], className="d-flex align-items-center mb-2"),
                    html.Div([
                        html.Img(src="/static/icon/humidity.svg", className="param-icon me-2"),
                        html.Span("HUMIDITY", className="param-title me-2"),
                        html.Span("70 %", className="param-value")
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
                    html.Span("400 PPM", className="param-value")
                ], className="d-flex align-items-center")
            ], className="param-card"), width=4),
        ], className="mb-3"),

        dbc.Row([
            dbc.Col(html.Div([
                html.Div([
                    html.Img(src="/static/icon/voltage.svg", className="param-icon me-2"),
                    html.Span("VOLTAGE", className="param-title me-2"),
                    html.Span("10 V", className="param-value")
                ], className="d-flex align-items-center")
            ], className="param-card"), width=4),

            dbc.Col(html.Div([
                html.Div([
                    html.Img(src="/static/icon/resistors.svg", className="param-icon me-2"),
                    html.Span("CURRENT", className="param-title me-2"),
                    html.Span("5 A", className="param-value")
                ], className="d-flex align-items-center")
            ], className="param-card"), width=4),

            dbc.Col(html.Div([
                html.Div([
                    html.Img(src="/static/icon/power.svg", className="param-icon me-2"),
                    html.Span("POWER", className="param-title me-2"),
                    html.Span("25 WATT", className="param-value")
                ], className="d-flex align-items-center")
            ], className="param-card"), width=4),
        ])
    ], className="container my-3"),

    # BOTTOM GRID
    html.Div([
        dbc.Row([
            dbc.Col(html.Img(src="/static/img/gh.jpg", className="greenhouse-img"), width=6),
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
                    html.Button("LOGOUT", id="logout-button", className="btn btn-dark"),
                    dcc.Location(id="logout-redirect", refresh=True)  # Handles redirection
                ], className="d-flex justify-content-end")
            ], width=6)
        ], className="g-2")
    ], className="container mb-5")
])

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
