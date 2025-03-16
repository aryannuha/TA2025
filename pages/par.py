from dash import dcc, html

par_layout = html.Div([
    html.H1("PAR Monitoring"),
    html.Div([
        html.Div([
            html.Div([
                html.Img(src='/assets/img/par.png', className='icon'),
                html.Div(id='par-display', className='data-box')
            ], className='data-item'),

            html.Div([
                html.Table([
                    html.Thead([
                        html.Tr([html.Th("Time"), html.Th("PAR Value")])
                    ]),
                    html.Tbody(id='par-table-body')
                ], className='data-table'),
            ], className='table-container'),

            html.Button("Historical Trend", id='btn-par', className='btn-his'),
        ], className='left-panel'),

        html.Div([
            dcc.Graph(id='par-graph', className='grafik'),
        ], className='right-panel'),
    ], className='main-container'),

    html.Div([
        html.Div([
            html.H2("Historical Trend"),
            dcc.Dropdown(
                id='par-filter-dropdown',
                options=[
                    {'label': 'Hour', 'value': 'hour'},
                    {'label': 'Day', 'value': 'day'},
                    {'label': 'Week', 'value': 'week'},
                    {'label': 'Month', 'value': 'month'}
                ],
                value='hour'
            ),
            dcc.Graph(id='par-historical-graph'),
            html.Button("Close", id='close-par-modal', className='close-btn')
        ], className='modal-content')
    ], id='par-modal', className='modal', style={'display': 'none'}),

    dcc.Interval(id='par-interval', interval=2000, n_intervals=0)
])