<template>
  <div>
    <Navigation
      :extension="generation.session.extension"
      :session="generation.session"
      :generation="generation"
    ></Navigation>
    <h1>Scores of Generation {{ generation.id }}</h1>
    <ChartComponent />
  </div>
</template>

<script>
import ChartComponent from "./Chart";
import Chart from "chart.js";
import Navigation from "../vue/Navigation";

export default {
  data() {
    return {
      generation: { session: { extension: null } },
      scores: [],
    };
  },
  mounted() {
    window.axios
      .get("/api/generation_scores/" + this.$route.params.id)
      .then((response) => {
        const data = response.data;
        this.scores = data.scores;
        this.generation = data.generation;

        var chart = new Chart("chart", {
          type: "bar",
          data: {
            labels: Array.from({ length: this.scores.length }, (_, i) => i + 1),
            datasets: [
              {
                label: "Scores",
                backgroundColor: "rgb(255, 99, 132)",
                borderColor: "rgb(255, 99, 132)",
                data: this.scores,
              },
            ],
          },
          options: { animation: { duration: 0 } },
        });
      });
  },
  components: {
    ChartComponent,
    Navigation,
  },
};
</script>