import '../sass/base.sass'
import { Elm } from '../elm/src/Main.elm'
import '@fortawesome/fontawesome-free/js/all.js'
import { exec } from 'child_process'

const path = require('path')
const toml = require('toml')
const fs = require('fs')

window.addEventListener('DOMContentLoaded', () => {
  try {
    const configFile = fs.readFileSync(path.resolve('../../config.toml'))
    const extensions = toml.parse(configFile).extensions

    const app = Elm.Main.init({ node: document.getElementById('main'), flags: extensions })

    app.ports.startExtension.subscribe(function (extension) {
      console.log(extension)
      exec('../../extensions/bin/neurogen/Contents/MacOS/NeuroGen', function (error, stdout, stderr) {
        if (error) {
          console.log(`error: ${error.message}`)
          return
        }
        if (stderr) {
          console.log(`stderr: ${stderr}`)
          return
        }
        console.log(stdout)
      })
    })
  } catch (e) {
    console.error('Parsing error on line ' + e.line + ', column ' + e.column +
      ': ' + e.message)
  }
})
