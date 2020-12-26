<template>
  <div>
    <Navigation
      :extension="session.extension"
      :session_scores="session"
    ></Navigation>
    <h1>Scores of Session {{ session.id }}</h1>
    <ChartComponent />
  </div>
</template>

<script>
import ChartComponent from "./Chart";
import ApexCharts from "apexcharts";
import Navigation from "../vue/Navigation";

export default {
  data() {
    return {
      session: {},
      scores: [],
    };
  },
  mounted() {
    window.axios
      .get("/api/session_scores/" + this.$route.params.id)
      .then((response) => {
        const data = response.data;
        this.scores = data.scores;
        this.session = data.session;

        var options = {
          chart: {
            type: "area",
            zoom: {
              type: "x",
              enabled: true,
              autoScaleYaxis: true,
            },
            toolbar: {
              autoSelected: "zoom",
            },
            animations: {
              enabled: false,
              easing: "easeinout",
              speed: 500,
              animateGradually: {
                enabled: true,
                delay: 150,
              },
              dynamicAnimation: {
                enabled: true,
                speed: 350,
              },
            },
          },
          stroke: {
            width: 7,
            curve: "smooth",
          },
          series: [
            {
              name: "Score",
              data: Array.from(this.scores, (number, _) => number.toFixed(2)),
            },
          ],
          xaxis: {
            categories: Array.from(
              { length: this.scores.length },
              (_, i) => i + 1
            ),
            title: {
              text: "Sessions",
            },
          },
          yaxis: {
            title: {
              text: "Scores",
            },
          },
        };

        var chart = new ApexCharts(document.querySelector("#chart"), options);

        chart.render();
      });
  },
  components: {
    ChartComponent,
    Navigation,
  },
};
</script>