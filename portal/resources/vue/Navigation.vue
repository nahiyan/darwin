<template>
  <nav aria-label="breadcrumb">
    <ol class="breadcrumb">
      <li
        :class="'breadcrumb-item' + (extensionActive ? ' active' : '')"
        aria-current="page"
      >
        <router-link :to="{ name: 'home' }">Extensions</router-link>
      </li>
      <li v-if="extension != null" class="breadcrumb-item" aria-current="page">
        <router-link :to="{ name: 'sessions', params: { id: extension.id } }"
          >Sessions of Extension <b>{{ extension.name }}</b></router-link
        >
      </li>
      <li v-if="session != null" class="breadcrumb-item" aria-current="page">
        <router-link :to="{ name: 'generations', params: { id: session.id } }"
          >Generations of Session <b>{{ session.id }}</b></router-link
        >
      </li>
      <li v-if="generation != null" class="breadcrumb-item" aria-current="page">
        <router-link
          :to="{ name: 'generation_scores', params: { id: generation.id } }"
          >Scores of Generation <b>{{ generation.id }}</b></router-link
        >
      </li>
    </ol>
  </nav>
</template>

<script>
export default {
  data() {
    return {
      extensionActive: false,
      sessionActive: false,
      generationActive: false,
    };
  },
  props: ["extension", "session", "generation"],
  mounted() {
    console.log(this.extension);
    if (this.extension == undefined) this.extensionActive = true;
    else if (this.session == undefined) this.sessionActive = true;
    else if (this.generation === undefined) this.generationActive = true;

    console.log(
      this.extensionActive,
      this.sessionActive,
      this.generationActive
    );
  },
};
</script>