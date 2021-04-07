const path = require('path')

module.exports = {
  entry: './src/js/index.js',
  target: 'electron-renderer',
  output: {
    filename: 'main.js',
    path: path.resolve(__dirname, 'dist')
  },
  module: {
    rules: [{
      test: /\.(sass)$/,
      use: [{
      // inject CSS to page
        loader: 'style-loader'
      }, {
      // translates CSS into CommonJS modules
        loader: 'css-loader'
      }, {
      // Run postcss actions
        loader: 'postcss-loader',
        options: {
        // `postcssOptions` is needed for postcss 8.x;
        // if you use postcss 7.x skip the key
          postcssOptions: {
          // postcss plugins, can be exported to postcss.config.js
            plugins: function () {
              return [
                require('autoprefixer')
              ]
            }
          }
        }
      }, {
      // compiles Sass to CSS
        loader: 'sass-loader'
      }]
    },
    {
      test: /\.elm$/,
      exclude: [/elm-stuff/, /node_modules/],
      use: {
        loader: 'elm-webpack-loader',
        options: {
          cwd: path.resolve(path.join('.', 'src', 'elm'))
        }
      }
    }]
  },
  devtool: 'cheap-module-source-map'
}
