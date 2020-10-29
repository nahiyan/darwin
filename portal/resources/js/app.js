import Vue from 'vue'

require('./bootstrap')
require('bootstrap')

var app = new Vue({
  el: '#app',
  data: {
    loaded: false,
    title: 'List of Extensions',
    extensions: [],
    sessions: [],
    generations: []
  },
  methods: {
    getSessions: (id) => {
      app.loaded = false
      window.axios.get('/api/sessions/' + id).then((response) => {
        app.loaded = true
        app.title = 'List of Sessions'
        app.extensions = []
        app.sessions = response.data
      })
    },
    getGenerations: (id) => {
      app.loaded = false
      window.axios.get('/api/generations/' + id).then((response) => {
        app.loaded = true
        app.title = 'List of Generations'
        app.sessions = []
        app.generations = response.data
      })
    }
  }
})

window.axios.get('/api/extensions').then((response) => {
  app.extensions = response.data
  app.loaded = true
})
