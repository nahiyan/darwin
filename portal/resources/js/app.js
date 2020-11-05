import Vue from 'vue'
import VueRouter from 'vue-router'
import Home from '../vue/Home'
import Sessions from '../vue/Sessions'
import Generations from '../vue/Generations'
import GenerationScores from '../vue/GenerationScores'
import SessionScores from '../vue/SessionScores'

require('./bootstrap')
require('bootstrap')

Vue.use(VueRouter)

const routes = [
  { path: '/', component: Home, name: 'home' },
  { path: '/extension/:id/sessions', component: Sessions, name: 'sessions' },
  { path: '/session/:id/generations', component: Generations, name: 'generations' },
  { path: '/session/:id/scores', component: SessionScores, name: 'session_scores' },
  { path: '/generation/:id/scores', component: GenerationScores, name: 'generation_scores' }
]

const router = new VueRouter({
  routes
})

const app = new Vue({
  router,
  data: {
    selection: {
      extension: null,
      session: null,
      generation: null
    }
  }
}).$mount('#app')
