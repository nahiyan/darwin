import Vue from 'vue'
import Chart from 'chart.js'

require('./bootstrap')
require('bootstrap')

var app = new Vue({
  el: '#app',
  data: {
    loaded: false,
    title: 'List of Extensions',
    selection: { extension: null, sessionId: null, generationId: null },
    extensions: [],
    sessions: [],
    generations: [],
    scores: []
  },
  methods: {
    getExtensions: () => {
      // app.loaded = false
      window.axios.get('/api/extensions').then((response) => {
        app.selection.extension = null
        app.selection.sessionId = null
        app.selection.generationId = null
        app.loaded = true
        app.title = 'List of Extensions'
        app.extensions = response.data
        app.sessions = []
        app.generations = []
        app.scores = []
      })
    },
    getSessions: (extension) => {
      // app.loaded = false
      window.axios.get('/api/sessions/' + extension.id).then((response) => {
        app.selection.extension = extension
        app.selection.sessionId = null
        app.loaded = true
        app.title = 'List of Sessions'
        app.extensions = []
        app.sessions = response.data
        app.scores = []
      })
    },
    getGenerations: (id) => {
      // app.loaded = false
      window.axios.get('/api/generations/' + id).then((response) => {
        app.selection.sessionId = id
        app.selection.generationId = null
        app.loaded = true
        app.title = 'List of Generations'
        app.sessions = []
        app.generations = response.data
        app.scores = []
      })
    },
    getPopulationScores: (id) => {
      // app.loaded = false
      window.axios.get('/api/generation_scores/' + id).then((response) => {
        app.selection.generationId = id
        app.loaded = true
        app.title = 'Statistics'
        app.generations = []
        app.scores = response.data

        const canvas = document.getElementById('population-scores').getContext('2d')
        var chart = new Chart(canvas, {
          // The type of chart we want to create
          type: 'line',

          // The data for our dataset
          data: {
            labels: [...Array(app.scores.length).keys()],
            datasets: [{
              label: 'Scores',
              backgroundColor: 'rgb(255, 99, 132)',
              borderColor: 'rgb(255, 99, 132)',
              data: app.scores
            }]
          },

          // Configuration options go here
          options: {}
        })
      })
    }
  }
})

app.getExtensions()
