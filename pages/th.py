from dash import dcc, html

th_layout = html.Div([
    html.H1("Temperature & Humidity Monitoring"),
    html.Div([
        html.Div([
            html.Div([
                html.Img(src='/assets/img/thermometer.png', className='icon'),
                html.Div(id='suhu-display', className='data-box')
            ], className='data-item'),

            html.Div([
                html.Img(src='/assets/img/humidity.png', className='icon'),
                html.Div(id='kelembaban-display', className='data-box')
            ], className='data-item'),

            html.Div([
                html.Table([
                    html.Thead([
                        html.Tr([html.Th("Time"), html.Th("Temperature (°C)"), html.Th("Humidity (%)")])
                    ]),
                    html.Tbody(id='table-body')
                ], className='data-table'),
            ], className='table-container'),

            html.Button("Historical Trend", id='btn-his', className='btn-his'),
        ], className='left-panel'),

        html.Div([
            dcc.Graph(id='suhu-graph', className='grafik'),
            dcc.Graph(id='kelembaban-graph', className='grafik'),
        ], className='right-panel'),
    ], className='main-container'),

    html.Div([
        html.Div([
            html.H2("Historical Trend"),
            dcc.Dropdown(
                id='filter-dropdown',
                options=[
                    {'label': 'Hour', 'value': 'hour'},
                    {'label': 'Day', 'value': 'day'},
                    {'label': 'Week', 'value': 'week'},
                    {'label': 'Month', 'value': 'month'}
                ],
                value='hour'
            ),
            dcc.Graph(id='historical-graph'),
            html.Button("Close", id='close-modal', className='close-btn')
        ], className='modal-content')
    ], id='modal', className='modal', style={'display': 'none'}),

    dcc.Interval(id='interval', interval=2000, n_intervals=0)
])