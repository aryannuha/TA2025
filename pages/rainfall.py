from dash import dcc, html

rainfall_layout = html.Div([
    html.H1("Rainfall Monitoring"),
    html.Div([
        html.Div([
            html.Div([
                html.Img(src='/assets/img/rainfall.png', className='icon'),
                html.Div(id='rainfall-display', className='data-box')
            ], className='data-item'),

            html.Div([
                html.Table([
                    html.Thead([
                        html.Tr([html.Th("Time"), html.Th("Rainfall Value")])
                    ]),
                    html.Tbody(id='rainfall-table-body')
                ], className='data-table'),
            ], className='table-container'),

            html.Button("Historical Trend", id='btn-rainfall', className='btn-his'),
        ], className='left-panel'),

        html.Div([
            dcc.Graph(id='rainfall-graph', className='grafik'),
        ], className='right-panel'),
    ],className='main-container'),

    html.Div([
        html.Div([
            html.H2("Historical Trend"),
            dcc.Dropdown(
                id='rainfall-filter-dropdown',
                options=[
                    {'label': 'Hour', 'value': 'hour'},
                    {'label': 'Day', 'value': 'day'},
                    {'label': 'Week', 'value': 'week'},
                    {'label': 'Month', 'value': 'month'}
                ],
                value='hour'
            ),
            dcc.Graph(id='rainfall-historical-graph'),
            html.Button("Close", id='close-rainfall-modal', className='close-btn')
        ], className='modal-content')
    ], id='rainfall-modal', className='modal', style={'display': 'none'}),

    dcc.Interval(id='rainfall-interval', interval=2000, n_intervals=0)
])