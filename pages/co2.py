from dash import dcc, html

co2_layout = html.Div([
    html.H1("CO2 Monitoring"),
    html.Div([
        html.Div([
            html.Div([
                html.Img(src='/assets/img/co2.png', className='icon'),
                html.Div(id='co2-display', className='data-box')
            ], className='data-item'),

            html.Div([
                html.Table([
                    html.Thead([
                        html.Tr([html.Th("Time"), html.Th("CO2 Value")])
                    ]),
                    html.Tbody(id='co2-table-body')
                ], className='data-table'),
            ], className='table-container'),

            html.Button("Historical Trend", id='btn-co2', className='btn-his'),
        ], className='left-panel'),

        html.Div([
            dcc.Graph(id='co2-graph', className='grafik'),
        ], className='right-panel'),
    ],className='main-container'),

    html.Div([
        html.Div([
            html.H2("Historical Trend"),
            dcc.Dropdown(
                id='co2-filter-dropdown',
                options=[
                    {'label': 'Hour', 'value': 'hour'},
                    {'label': 'Day', 'value': 'day'},
                    {'label': 'Week', 'value': 'week'},
                    {'label': 'Month', 'value': 'month'}
                ],
                value='hour'
            ),
            dcc.Graph(id='co2-historical-graph'),
            html.Button("Close", id='close-co2-modal', className='close-btn')
        ], className='modal-content')
    ], id='co2-modal', className='modal', style={'display': 'none'}),

    dcc.Interval(id='co2-interval', interval=2000, n_intervals=0)
])