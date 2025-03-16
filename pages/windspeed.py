from dash import dcc, html

windspeed_layout = html.Div([
    html.H1("Windspeed Monitoring"),
    html.Div([
        html.Div([
            html.Div([
                html.Img(src='/assets/img/windspeed.png', className='icon'),
                html.Div(id='windspeed-display', className='data-box')
            ], className='data-item'),

            html.Div([
                html.Table([
                    html.Thead([
                        html.Tr([html.Th("Time"), html.Th("Windspeed Value")])
                    ]),
                    html.Tbody(id='windspeed-table-body')
                ], className='data-table'),
            ], className='table-container'),

            html.Button("Historical Trend", id='btn-windspeed', className='btn-his'),
        ], className='left-panel'),

        html.Div([
            dcc.Graph(id='windspeed-graph', className='grafik'),
        ], className='right-panel'),
    ], className='main-container'),

    html.Div([
        html.Div([
            html.H2("Historical Trend"),
            dcc.Dropdown(
                id='windspeed-filter-dropdown',
                options=[
                    {'label': 'Hour', 'value': 'hour'},
                    {'label': 'Day', 'value': 'day'},
                    {'label': 'Week', 'value': 'week'},
                    {'label': 'Month', 'value': 'month'}
                ],
                value='hour'
            ),
            dcc.Graph(id='windspeed-historical-graph'),
            html.Button("Close", id='close-windspeed-modal', className='close-btn')
        ], className='modal-content')
    ], id='windspeed-modal', className='modal', style={'display': 'none'}),

    dcc.Interval(id='windspeed-interval', interval=2000, n_intervals=0)
])